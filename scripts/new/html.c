#include <stdio.h>
#include<stdlib.h>

#include "node.h"

char
    *monthname[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
        "Aug", "Sep", "Oct", "Nov", "Dec"},

    *prefix = "<br>&nbsp;&nbsp;&nbsp;";


void show_links (struct NodeLink *nlink, char *name) 
{
    if (nlink->next == NULL)
        printf ("%s%s : <a href=\"#%s\">%s</a>\n", prefix, name,
            nlink->node->id, nlink->node->name);
    else
    {
        printf ("%s%s : \n", prefix, name);

        while (nlink)
        {
            printf ("%s&nbsp;&nbsp;&nbsp;<a href=\"#%s\">%s</a>\n",
                prefix, nlink->node->id, nlink->node->name);

            nlink = nlink->next;
        }
    }
}


int main (int argc, char *argv[])
{
    struct Node
        *nlist;

    struct NodeLink
        *nlink;

    argc--;
    argv++;
    nlist = parse_files (argc, argv);


    printf ("<html>\n<head>\n</head>\n<body>\n");

    while (nlist)
    {
        printf ("<p><a name=\"%s\">%s</a>\n", nlist->id, nlist->name);
        printf ("%sType : %s\n", prefix, nlist->type);

        if (nlist->date)
        {
            printf ("%sDate : %d", prefix, nlist->date->year);

            if (nlist->date->month)
            {
                int
                    n = nlist->date->month;

                printf (" %s", monthname[(n <= 12) ? n : 0]);
                
                if (nlist->date->day)
                    printf (" %d", nlist->date->day);
            }
            printf ("\n");
        }

        if (NULL != (nlink = nlist->reference))
        {
            if (nlink->next == NULL)
                printf ("%sReference : %s", prefix, (char *)(nlink->node));
            else
            {
                printf ("%sReferences :\n", prefix);
                while (nlink)
                {
                    printf ("%s&nbsp;&nbsp;&nbsp;%s\n", prefix, 
                        (char*)(nlink->node));                        

                    nlink = nlink->next;
                }
            }
        }

        if (nlist->status)
            printf ("%sStatus : %s", prefix, nlist->status);

        if (NULL != (nlink = nlist->aka))
            show_links (nlink, "Aka");

        if (NULL != (nlink = nlist->successorto))
            show_links (nlink, "Successor to");

        if (NULL != (nlink = nlist->codetakenfrom))
            show_links (nlink, "Code taken from");

        if (NULL != (nlink = nlist->influencedby))
            show_links (nlink, "Influenced by");

        if (NULL != (nlink = nlist->runson))
            show_links (nlink, "Runs on");

        if (NULL != (nlink = nlist->ranon))
	  show_links (nlink, "Ran on");

        if (NULL != (nlink = nlist->madeby))
	  show_links (nlink, "Made by");

        printf ("\n<br><hr><br>\n");
        nlist = nlist->next;
    }


    printf ("\n</body></html>\n");
    return 0;
}



