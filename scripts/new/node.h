/*---------------------------------------------------------------------------
										Field definitions
---------------------------------------------------------------------------*/

/* This table is used to define the field names and the Node struct fields

   The function name specifies the type of field : STR for string, LINK for 
   links, DATE for dates or ENUM for fields whose values are to be checked.

   The first parameter is 1 or 0 depending on whether that field is
   mandatory. If the field is not specified and the value is 1 then an 
   error is declared.

	The second parameter defines the string used to introduce this field.
   This string is case sensitive.

   The third parameter specifies the Node struct field name. STR and 
   ENUM fields are declared as 'char *', LINK fields as 'struct Node *'
   and DATE fields as 'struct Date *'

	The order of the fields in the Node struct is the same as the order
   order specified here.

   For ENUM fields, an additional line is required defining the values
   to be accepted for that field. This should be an array of pointer to
   strings with the strings being the valid values. Aterminating NULL
   must be provided. The name of theis variable should be the struct field
   name concatenated with "Values" (see example below).
 */

#define FIELD_DEFS \
   FSTR  (1, "Name:",            name ) \
   FSTR  (0, "Reference:",       reference ) \
   FENUM (1, "Type:",            type )\
   FSTR  (0, "Status:",          status ) \
   FLINK (0, "Aka ",             aka ) \
   FLINK (0, "Successor to ",    successorto ) \
   FLINK (0, "Code taken from ", codetakenfrom ) \
   FLINK (0, "Influenced by ",	influencedby ) \
   FLINK (0, "Runs on ",         runson ) \
   FDATE (0, "Date:",            date ) \
   FDATE (0, "Founded on ",      foundedon ) 

char  

	/* Valid values of 'type' field */

	*typeValues[] = {"OS", "os", "hardware", "language", "standard", 
		"announcement", "company", NULL};


/*---------------------------------------------------------------------------
										 Node definition
---------------------------------------------------------------------------*/

struct Date {
	unsigned char
		day,
		month;

	unsigned int 
		year;
};


#define FSTR(r,s,n)   char * n;
#define FLINK(r,s,n)  struct Node * n;
#define FDATE(r,s,n)  struct Date * n;
#define FENUM(r,s,n)  char * n;

struct Node {
	char *id;
	struct Node *next;

	void *field[1];

   FIELD_DEFS
};

#undef FSTR
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


enum f_types { UNK, STR, LINK, DATE, ENUM }; 

struct fstruct {
	enum f_types typ;
   char rqd_flag;
	char *fname;
   char **values;
};

#ifdef PARSER

#define FSTR(r,s,n) {STR, r, s, NULL}, 
#define FLINK(r,s,n) {LINK, r, s, NULL}, 
#define FDATE(r,s,n) {DATE, r, s, NULL}, 
#define FENUM(r,s,n) {ENUM, r, s, n##Values},
 
struct fstruct fields [] = {
	FIELD_DEFS
   {UNK, 0, "", NULL}
};

#undef FSTR
#undef FLINK
#undef FDATE
#undef FENUM

#endif

#undef FIELDEFS
