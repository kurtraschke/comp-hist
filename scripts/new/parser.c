#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PARSER
#include "node.h"

#define LINE_LENGTH 512


static char
	current_line[LINE_LENGTH],			/* The current line */
	*current_pos = NULL;					/* Pointer into current_line */


struct LinkList {
	void *linkto;
	struct LinkList *next;
};


static char *dupstr (const char *s)
{
	return strcpy ((char *)malloc (strlen (s) + 1), s); 
}

int
	error_count;


void error (int severity, char *errmsg)
{
	char
		*lp = current_line;

	if (current_pos)
	{
		fprintf (stderr, "%s", current_line);

		while (lp < current_pos)
			fprintf (stderr, "%c", (*lp++ == '\t') ? '\t' : ' ');
			}

	fprintf (stderr, "^\n%s\n", errmsg);

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
		ts = s;

		/* if the line is more than LINE_LENGTH long, we must skip the remaining of the line */
		if (strlen(s)==LINE_LENGTH-1)
			while (fgetc(f)!='\n') {}; 


		if ((*ts == '#') || (*ts == '\n'))
			continue;

		while ((*ts == ' ') || (*ts == '\t') || (*ts == '\n'))
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

	skip_spaces ();

	*d = (struct Date *)malloc (sizeof (struct Date));

	(*d)->month = (*d)->day = 0;

	n = 0;
	while (isdigit (*current_pos))
		n = (n * 10) + (*current_pos++ - '0');

	if ((n < 1900) || (n > 2000))
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



static void copy_eol (char **s)
{
	/* Copies characters form the current_pos up to the end of line or
		'#' char whichever comes first.
		Trailing spaces are not copied.
	 */

	char
		savech,
		*ts = current_pos;

	skip_spaces ();

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

	*s = dupstr (current_pos);

	*ts = savech;
}



void add_string(struct LinkList **l)
{
	struct LinkList *tmp;
	
	tmp=(struct LinkList *)malloc(sizeof(struct LinkList));
	copy_eol((char **)&(tmp->linkto) );
	tmp->next=*l;
	*l=tmp;
}



static struct Node *add_new_node (struct Node *list)
{
	/* adds a new node to list, returning the new list head */

	struct Node
		*tmp;

	int
		fn;

	tmp = (struct Node *) malloc (sizeof (struct Node));
	copy_eol (&tmp->id);
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
		**vv;
		
	skip_spaces ();

	while (0 != (n = strlen (fields[i].fname)))
	{
		if (!strncmp (current_pos, fields[i].fname, n))
		{
			current_pos += n;

			switch (fields[i].typ)
			{
			case STR:
				copy_eol ((char **)&(node->field[1+i]));
				return;

			case LINK:				
				add_string((struct LinkList **)&(node->field[1+i]));
				return;

			case DATE:
				read_date ((struct Date **)&(node->field[1+i]));
				return;
				
			case ENUM:
				copy_eol ((char **)&(node->field[1+i]));

				vv = fields[i].values;

				while (*vv)
				{
					if (strcmp (*vv, (char *)(node->field[1+i])) == 0)
						break;

					vv++;
				}

				if (*vv == NULL)
				error (0, "Invalid field value");

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

	struct LinkList
		*tmp2=NULL;

	int
		fn;

	error_count = 0;

	while (argc--)
	{
		if ((f = fopen (*argv++, "r")) == NULL)
			error (1, "cannot open file");

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

	/*	Now go through the list checking for duplicate node ids,
		checking that required fileds have been specified and 
		replacing links which are currently string names with the
		appropriate Node pointer
	 */

	t = node_list;
	while (t)
	{
		/* Check that node id is unique */

		if (seek_node (node_list, t->id) != t)
		{
			error_count++;
			fprintf (stderr, "Duplicate node id '%s'\n", t->id);
		}

		/* Scan the field definitions, check required fields are specified
			and for each LINK field, set that field to be a pointer to the
			node
		 */

		for (fn = 0; fields[fn].typ != UNK;)
		{

			/* If field is required make sure it has a value */

			if (fields[fn].rqd_flag && (t->field[fn+1] == NULL))
			{
				fprintf (stderr, "Node %s, field %s not specified\n",
					t->id, fields[fn].fname);
				error_count++;
			}


			if ((fields[fn].typ == LINK) && t->field[fn+1])
			{
				tmp2=(struct LinkList *)t->field[fn+1];
				while (tmp2) {
					if (NULL == (tmp = seek_node (node_list, (char *)(tmp2->linkto))))
					{
						fprintf (stderr, "Cannot find node '%s'\n", (char *)(tmp2->linkto));
						error_count++;
					}
					free((char *)(tmp2->linkto));
					
					(struct Node *)(tmp2->linkto) = tmp;
					tmp2=tmp2->next;
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

	struct LinkList *
		tmp;

	printf ("\n%s\n", n->id);

	for (fn = 0; fields[fn].typ != UNK; fn++)
		if (n->field[fn+1])
			switch (fields[fn].typ)
			{
			case STR:
			case ENUM:
				printf ("\t%s %s\n", fields[fn].fname, (char *)(n->field[fn+1]));
				break;

			case DATE:
				printf ("\t%s ", fields[fn].fname);
				print_date ((struct Date *)(n->field[fn+1]));
				break;

			case LINK:
				tmp=(struct LinkList *)n->field[fn+1];
				while (tmp) {
					printf ("\t%s %s\n", fields[fn].fname,
						((struct Node *)(tmp->linkto))->id);
					tmp=tmp->next;
				};
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
		fprintf (stderr, "%d error encountered\n", error_count);

	/* Display nodes... */

	while (t)
	{
		disp_node (t);
		t = t->next;
		}

	return 0;
}

#endif
