/*---------------------------------------------------------------------------
							 User definitions
---------------------------------------------------------------------------*/

/* The defintion of UNKNOWNFIELDS controls the actions of the parser
   on encountering a filed which is not matched by any of the field 
   defintions.

   If UNKNOWNFIELDS is 0, the parser will silently discard the unknown
   field definiton.

   If UNKNOWFIELDS is 1, the parser will establish an MSTR at the 
   item field in the node structure and save the entire line (excluding
   leading/trailing whitespace and comments) in that field. This allows
   for a ser program to recover the unknown field values.

   If UNKNOWNFILEDS is -1, then parser will flag such fields as errors.
 */

#ifndef UNKNOWNFIELDS
#define UNKNOWFIELDS 1
#endif

/* MAX_YEAR defines the maximum value permitted as the year of a Date */

#ifndef MAX_YEAR
#define MAX_YEAR    2001
#endif

/* LINELENGTH defines the size of the longest supported line form the 
   input file
 */

#ifndef LINE_LENGTH
#define LINE_LENGTH 1024
#endif


/* This table is used to define the field names and the Node struct fields

   The function name specifies the type of field : STR for string, LINK for 
   links, DATE for dates or ENUM for fields whose values are to be checked.
   The MSTR type allows multiple strings to be stored on a linked list like
   the LINKS.

   The first parameter is a flag value to control error checking etc.
   The field is bit encoded, values are given towards the end of the file.

       Bit 0 (value 1) if set makes the field mandatory, ie generate an 
       error if the field is not specified in a node.
       
       Bit 1 (value 2) if set allows the field to be specified multiple
       times, when clear the field must be specified at most once.
       (Note that if this flag is set, the parser will accpet multiple 
        fields but will only save the multiples for LINK or MSTR fields)

   The second parameter defines the string used to introduce this field.
   This string is case sensitive.

   The third parameter specifies the Node struct field name. STR and 
   ENUM fields are declared as 'char *', LINK fields as 'struct Node *'
   and DATE fields as 'struct Date *'

	The order of the fields in the Node struct is the same as the order
   order specified here.

   For ENUM fields, an additional line is required defining the values
   to be accepted for that field. This should be an array of pointer to
   strings with the strings being the valid values. A terminating NULL
   must be provided. One more value after the NULL must be provided, this
   is the value which will bu placed in the field if its read value is
   not one on the list. The name of this variable should be the struct 
   field name concatenated with "Values" (see example below).
 */

#define FIELD_DEFS \
   FSTR  (1, "Name:",            name )				\
   FENUM (1, "Type:",            type )				\
   FDATE (0, "Date:",            date )				\
   FMSTR (2, "Reference:",       reference )		\
   FSTR  (0, "Status:",          status )			\
   FLINK (2, "Aka ",             aka )				\
   FLINK (2, "Successor to ",    successorto )		\
   FLINK (2, "Code taken from ", codetakenfrom )	\
   FLINK (2, "Influenced by ",   influencedby )		\
   FLINK (2, "Runs on ",         runson )			\
   FDATE (0, "Founded on ",      foundedon ) 

#ifdef PARSER

char  
    /* Specify here Field ids which should be ignored, that is
       neither stored in the node list nor cause an error 
     */

    *ignore_fields[] = {
        "Info:",
        NULL };
    
	/* Valid values of 'type' field */

char
	*typeValues[] = {"OS", "hardware", "language", "standard", 
		"announcement", "company", NULL, "??"};

#endif

/*                    END OF USER DEFINITION                               */ 

/*---------------------------------------------------------------------------
								 Node definition
---------------------------------------------------------------------------*/

enum f_types { UNK, STR, MSTR, LINK, DATE, ENUM }; 


struct Date {
	unsigned char
		day,
		month;

	unsigned int 
		year;
};


#define FSTR(r,s,n)   char * n;
#define FMSTR(r,s,n)  struct NodeLink * n;
#define FLINK(r,s,n)  struct NodeLink * n;
#define FDATE(r,s,n)  struct Date * n;
#define FENUM(r,s,n)  char * n;


struct NodeLink {
    struct Node *node;
    struct NodeLink *next;
};


struct Node {
	char *id;
	struct Node *next;

	void *field[1];

   FIELD_DEFS
};

#undef FSTR
#undef FMSTR
#undef FLINK
#undef FDATE
#undef FENUM

#ifndef PARSER

/* This section defines a constant for each data field in a node.
   This constant defines the format of that datafield in terms of 
   the f_types enumeration.
 */

#define FSTR(r,s,n)     const enum f_types n##_typ = STR;
#define FMSTR(r,s,n)    const enum f_types n##_typ = MSTR;
#define FLINK(r,s,n)    const enum f_types n##_typ = LINK;
#define FDATE(r,s,n)    const enum f_types n##_typ = DATE;
#define FENUM(r,s,n)    const enum f_types n##_typ = ENUM;

FIELD_DEFS    
    
#endif

#undef FSTR
#undef FMSTR
#undef FLINK
#undef FDATE
#undef FENUM

/*---------------------------------------------------------------------------
										 Public routines
---------------------------------------------------------------------------*/

struct Node *parse_files (int argc, char **argv); /* Parse a list of files */

struct Node *seek_node (struct Node *nodes, char *s); /* Find node by id */


#if defined(DISPLAY)
void disp_node (struct Node *n);		/* Print node n to standard out. */
#endif

/*-------------------------------------------------------------------------*/

#ifdef PARSER

struct fstruct {
	enum f_types typ;
	char flags;
	char *fname;
	char **values;
};


/* Values for fstruct flags */

#define REQUIRED        0x01
#define ALLOWDUPS       0x02


/* Define functions to generate field description table */

#define FSTR(r,s,n)     {STR,  r, s, NULL}, 
#define FMSTR(r,s,n)    {MSTR, r, s, NULL},
#define FLINK(r,s,n)    {LINK, r, s, NULL}, 
#define FDATE(r,s,n)    {DATE, r, s, NULL}, 
#define FENUM(r,s,n)    {ENUM, r, s, n##Values},
 
struct fstruct fields [] = {
   {MSTR, 2, "", NULL},
	FIELD_DEFS
   {UNK, 0, "", NULL}
};

#undef FSTR
#undef FLINK
#undef FDATE
#undef FENUM
#undef FIELD_DEFS

/* Define the pool sizes for allocating the various data structures */

#define NODEPOOL        500     /* struct Node */
#define DATEPOOL        1000    /* struct Date */
#define NODELINKPOOL    1000    /* struct NodeLink */
#define STRINGPOOL      8192    /* char */

#else   /* Not parser so undef all non parser defines */

#undef MAX_YEAR
#undef LINE_LENGTH
#undef FIELD_DEFS

#endif
