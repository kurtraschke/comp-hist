#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PARSER			/* defined to build field definition tables */ 
#include "node.h"


static char
	current_line[LINE_LENGTH],			/* The current line */
	*current_pos = NULL,				/* Pointer into current_line */
	*current_file = NULL;               /* Name of current file */

static int
	line_count = 0;                     /* Line number in current file */


int 
	error_count = 0;                    /* Count of errors encountered */



static void out_of_memory (char *proc)
{
	fprintf (stderr, "Out of memory %s %s[%d] - exiting\n", proc, 
	    current_file, current_line);	
	exit (10);
}
	

static char *dupstr (const char *s)
{
    static int
        stringcount = 0;

    static char*
        stringpool;

    char 
        *t;

    int
        sl = strlen (s) + 1;

    if (sl > stringcount)
    {
        stringpool = (char *) malloc (stringcount = STRINGPOOL);

        if (stringpool == NULL)
            out_of_memory ("allocating stringpool");
    }

    t = stringpool;
    stringpool += sl;
    stringcount -= sl;

	return strcpy (t, s);
}


void error (int severity, char *errmsg)
{
	char
		*lp = current_line;

	if (current_pos)
	{
		fprintf (stderr, "\n%s", current_line);

		while (lp < current_pos)
			fprintf (stderr, "%c", (*lp++ == '\t') ? '\t' : ' ');
	}

	fprintf (stderr, "^\n");
		
	if (current_file)
		fprintf (stderr, "%s[%d] ", current_file, line_count);

	fprintf (stderr, "%s\n", errmsg);

	error_count++;

	if (severity)
		exit (1);
}


static int short get_line (char *s, FILE *f)
{
	/* reads a new line in the file, ignores blank lines and lines
		beginning with '#', replaces all leading whitespace with a
		single space
	 */

	char
		*ts = s;

	while (0 < fgets (s, LINE_LENGTH, f))
	{
		if (!feof (f) && (*(s + strlen (s) - 1) != '\n'))
			error (0, "Line too long");
		else
			line_count++;

		ts = s;

		if ((*ts == '#') || (*ts == '\n'))
			continue;

		while (( by == ' ') || (*ts == '\t') || (*ts == '\n'))
			ts++;

		if (*ts == '#')
			continue;

		current_pos = (ts > s) ? (*--ts = ' ', ts) : s;

		return 0;
	}

	return -1;
}

 
static void skip_spaces (void)
{
	/* positions current_pos at the beginning of a word 
		ie skips tabs and spaces
	 */

	while ((*current_pos == ' ') || (*current_pos == '\t'))
		current_pos++;
}


static void read_date (struct Date **d)
{
	/* reads a date from current_pos */

	int
		n;

    static struct Date
        *datepool;

    static int
        datecount = 0;

	skip_spaces ();


    if (datecount == 0)
    {
        datepool = (struct Date *) malloc ((datecount = DATEPOOL) * 
            sizeof (struct Date));

        if (datepool == NULL)
            out_of_memory ("allocating datepool");
    }

    *d = datepool++;
    datecount--;

	(*d)->month = (*d)->day = 0;

	n = 0;
	while (isdigit (*current_pos))
		n = (n * 10) + (*current_pos++ - '0');

	if ((n < 1900) || (n > MAX_YEAR))
		error (0, "Year out of range");

	(*d)->year = n;

	if ((*current_pos == '-') || (*current_pos == '/'))
	{
		current_pos++;
		n = 0;
		while (isdigit (*current_pos))
			n = n * 10 + (*current_pos++ - '0');

		if ((n < 1) || (n > 12))
			error (0, "Month out of range");

		(*d)->month = n;

		if ((*current_pos == '-') || (*current_pos == '/'))
		{
			current_pos++;
			n = 0;
			while (isdigit (*current_pos))
				n = n * 10 + (*current_pos++ - '0');

			if ((n < 1) || (n > 31))
				error (0, "Day out of range");

			(*d)->day = n;
		}
	}
}


static struct Node *add_new_node (struct Node *list)
{
	/* adds a new node to list, returning the new list head */

	struct Node
		*tmp;

	int
		fn;

    char
        savech,
        *ts;

    static int
        nodecount = 0;

    static struct Node
        *nodepool;

    if (nodecount == 0)
    {
        nodepool = (struct Node *) malloc ((nodecount = NODEPOOL) *
            sizeof (struct Node));

        if (nodepool == NULL)
            out_of_memory ("nodepool allocation");
    }

    tmp = nodepool++;
    nodecount--;

    ts = current_pos;
    while (*ts && (*ts != '#') && !isspace (*ts))
        ts++;

    savech = *ts;
    *ts = '\0';
    tmp->id = dupstr (current_pos);
    *ts = savech;

	if (seek_node (list, tmp->id))
		error (0, "Duplicate node id");
		
	tmp->next = list;

	/* Clear all field contnets */

	for (fn = 0; fields[fn].typ != UNK; fn++)
	   tmp->field[fn+1] = NULL;

	return tmp;
}


void parse_line (struct Node *node)
{
	/* the main function which analyzes a field line */


	/* determine if there is an identifier which is a prefix of s */
	/* returns the position of the first character after the field name */
	/* if s begins with a field name */
	/* returns 0 otherwise */
	/* the comparison is case sensitive */

	int
		n,
		i = 0;

	char
		savech,
        *s,
		*ts,
		**vv;
		
    struct NodeLink
        *nl;

    static struct NodeLink
        *nodelinkpool;

    static int
        nodelinkcount = 0;


	skip_spaces ();

	while (0 != (n = strlen (fields[i].fname)))
	{
		if (!strncmp (current_pos, fields[i].fname, n))
		{
			current_pos += n;

			if (((fields[i].flags & ALLOWDUPS) == 0) && node->field[i + 1])
                error (0, "Duplicate field");

        	skip_spaces ();
            ts = current_pos;

        	/* Find end of line, # or \n */

        	while (*ts && (*ts != '#') && (*ts != '\n'))
        		ts++;

        	/* Trim any trailing whitespace */

        	ts--;
        	while (((*ts == '\t') || (*ts == ' ')) && (ts > current_pos))
        		ts--;

        	/* And terminate the string */

        	savech = *++ts;
        	*ts = '\0';

            switch (fields[i].typ)
			{
			case STR:
                (char *)(node->field[1+i]) = dupstr (current_pos);
                *(current_pos = ts) = savech;
                return;

			case LINK:
            case MSTR:
                /* All link fields are implemented using NodeLinks which
                    allow multiple links.
                 */
                    
                if (nodelinkcount == 0)
                {
                    nodelinkpool = (struct NodeLink *) malloc (
                        (nodelinkcount = NODELINKPOOL) * sizeof (
                        struct NodeLink));

                    if (nodelinkpool == NULL)
                           out_of_memory ("allocating nodelinkpool");
                }

                /* Here we add the new nodelink to the end of the chain,
                   or to the head if it is empty. This ensures that for
                   multiple field specifications, the order in the list is
                   the same as the order in the file.
                 */

                if ((nl = (struct NodeLink *)(node->field[1+i])) == NULL)
                    nl = (struct NodeLink *)(node->field[1+i]) = 
                        nodelinkpool++;
                else
                {
                    while (nl->next)
                        nl = nl->next;

                    nl->next = nodelinkpool++;
                    nl = nl->next;
                }

                nl->next = NULL;
                nodelinkcount--;
               
                (char *)(nl->node) = dupstr (current_pos);
                *(current_pos = ts) = savech;

				return;

			case DATE:
				read_date ((struct Date **)&(node->field[1+i]));
                *ts = savech;
				return;

			case ENUM:
				vv = fields[i].values;

				while (*vv)
				{
					if (strcmp (*vv, current_pos) == 0)
					{
						(char *)(node->field[1+i]) = *vv;
						break;
					}
					vv++;
				}

				if (*vv == NULL)
					error (0, "Invalid field value - field ignored");
			}

			return;
		}

		i++;
	}
}


struct Node *seek_node (struct Node *nodes, char *s)
{
	/* returns the node which identifier is s */

	while (nodes && (0 != strcmp (s, nodes->id)))
		nodes = nodes->next;

	return nodes;
}


struct Node *parse_files (int argc, char **argv)
{
	/* parses an entire file adding the parsed content to node_list */

	FILE
		*f;

	struct Node
		*node_list = NULL,
		*tmp,
		*t;

    struct NodeLink
        *nl;

	int
		fn;

	error_count = 0;

	while (argc--)
	{
		if ((f = fopen (*argv, "r")) == NULL)
			error (1, "cannot open file");

		current_file = *argv++;
		line_count = 0;

		get_line (current_line, f);

		if (*current_pos == ' ')
			error (1, "Expected node id");

		while (1)
		{
			if (*current_pos != ' ')
				node_list = add_new_node (node_list);
			else
				parse_line (node_list);

			if (get_line (current_line, f) == -1)
				break;
		}

		fclose (f);
	}

	/*	Now go through the list checking that required fields have been
		specified and replacing links which are currently string names
		with the appropriate Node pointer
	 */

	t = node_list;
	while (t)
	{
		/* Scan the field definitions, check required fields are specified
			and for each LINK field, set that field to be a pointer to the
			node
		 */

		for (fn = 0; fields[fn].typ != UNK;)
		{
			/* If field is required make sure it has a value */

			if ((fields[fn].flags & REQUIRED) && (t->field[fn+1] == NULL))
			{
				fprintf (stderr, "Node %s, field %s not specified\n",
					t->id, fields[fn].fname);
				error_count++;
			}

			if ((fields[fn].typ == LINK) && t->field[fn+1])
			{
                nl = (struct NodeLink *)(t->field[fn+1]);
                
                while (nl)
                {
				    if (NULL == (tmp = seek_node (node_list, 
				        (char *)(nl->node))))
				    {
					    fprintf (stderr, "%s : Cannot find %s node '%s'\n",
						    t->id, fields[fn].fname, (char *)(nl->node));
		 			    error_count++;
				    }

				    nl->node = tmp;
				    nl = nl->next;
                }
			}

			fn++;
		}

		t = t->next;
	}

	return node_list;
}


#if defined(DISPLAY) || defined(TESTING)

/*---------------------------------------------------------------------------
										 Display Routines
---------------------------------------------------------------------------*/

static void print_date (struct Date *d)
{
	/* prints a date */

	printf ("%u", d->year);
	
	if (d->month)
		printf ("/%u", d->month);
	
	if (d->day)
		printf ("/%u", d->day);

	printf ("\n");
}


void disp_node (struct Node *n)
{
	int
		fn;

    struct NodeLink
        *nl;

	printf ("\n%s\n", n->id);

	for (fn = 0; fields[fn].typ != UNK; fn++)
		if (n->field[fn + 1])
			switch (fields[fn].typ)
			{
			case STR:
			case ENUM:
				printf ("\t%s %s\n", fields[fn].fname, 
				    (char *)(n->field[fn + 1]));
				break;

			case DATE:
				printf ("\t%s ", fields[fn].fname);
				print_date ((struct Date *)(n->field[fn + 1]));
				break;

            case MSTR:
                nl = (struct NodeLink *)(n->field[fn + 1]);
                while (nl)
                {
                    printf ("\t%s%s\n", fields[fn].fname, 
                        (char*)(nl->node));

                    nl = nl->next;
                }

                break;

			case LINK:
                nl = (struct NodeLink *)(n->field[fn + 1]);

                while (nl)
                {
                    printf ("\t%s%s\n", fields[fn].fname,
					    nl->node->id);
                    nl = nl->next;
                }

				break;
			}

	printf ("\n");
}

#endif


#ifdef TESTING

int main (int argc, char **argv)
{
	struct Node
		*t;

	if (argc < 2)
	{
		printf ("parser <file> ...");
		exit (1);
	}

	t = parse_files (--argc, ++argv);

	if (error_count)
		fprintf (stderr, "%d error%s encountered\n", error_count,
			(error_count > 1) ? "s" : "");

	/* Display nodes... */

	while (t)
	{
		disp_node (t);
		t = t->next;
	}

	return 0;
}

#endif
