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


void node_to_html(struct Node *node)
{

   struct NodeLink
        *nlink;



        if (node==NULL) return;

        printf ("<p><a name=\"%s\">%s</a>\n", node->id, node->name);
        printf ("%sType : %s\n", prefix, node->type);

        if (node->date)
        {
            printf ("%sDate : %d", prefix, node->date->year);

            if (node->date->month)
            {
                int
                    n = node->date->month;

                printf (" %s", monthname[(n <= 12) ? n : 0]);
                
                if (node->date->day)
                    printf (" %d", node->date->day);
            }
            printf ("\n");
        }

        if (NULL != (nlink = node->reference))
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

        if (node->status)
            printf ("%sStatus : %s", prefix, node->status);

        if (NULL != (nlink = node->aka))
            show_links (nlink, "Aka");

        if (NULL != (nlink = node->successorto))
            show_links (nlink, "Successor to");

        if (NULL != (nlink = node->codetakenfrom))
            show_links (nlink, "Code taken from");

        if (NULL != (nlink = node->influencedby))
            show_links (nlink, "Influenced by");

        if (NULL != (nlink = node->runson))
            show_links (nlink, "Runs on");

        printf ("\n<br><hr><br>\n");

        node_to_html(node->left);
        node_to_html(node->right);
}

int main (int argc, char *argv[])
{
    struct Node
        *nlist;

    argc--;
    argv++;
    nlist = parse_files (argc, argv);


    printf ("<html>\n<head>\n</head>\n<body>\n");

    node_to_html(nlist);

    printf ("\n</body></html>\n");
    return 0;
}



