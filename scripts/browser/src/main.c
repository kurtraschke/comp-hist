/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

#include "node.h"

GtkWidget *text;

void gtk_text_clear(GtkText *text)
{
	gtk_text_set_point(text,0);
	gtk_text_forward_delete(GTK_TEXT(text),gtk_text_get_length(GTK_TEXT(text)));
}

void print_text(GtkText *text,gchar *s)
{
	gtk_text_insert(text,NULL,NULL,NULL,s,-1);
}

void print_text_newline(GtkText *text,gchar *s)
{
	gtk_text_insert(text,NULL,NULL,NULL,s,-1);
	gtk_text_insert(text,NULL,NULL,NULL,"\n",-1);

}

gchar *date_to_str(struct Date *d)
{

	if (d->day) {
		return g_strdup_printf("%u/%u/%u",d->year,d->month,d->day);
	};

	if (d->month) {
		return g_strdup_printf("%u/%u",d->year,d->month);
	};
	
	return g_strdup_printf("%u",d->year);
}

void print_node(GtkText *text,struct Node *node)
{
	int fn;

	struct NodeLink	*nl;

	print_text_newline(text,node->id);

	for (fn = 1; ; fn++, (fn = ((fields[fn].typ == UNK) ? 0 : fn)))
	{
		if (node->field[fn])
			switch (fields[fn].typ)
			{
			case STR:
			case ENUM:
				print_text(text,"\t");
				print_text(text,fields[fn].fname);
				print_text(text," ");
				print_text_newline(text,(char *)(node->field[fn]));
				break;

			case DATE:
				print_text(text,"\t");
				print_text(text, fields[fn].fname);
				print_text(text," ");
				print_text_newline(text, date_to_str((struct Date *)(node->field[fn])));
				break;

			case MSTR:
				nl = (struct NodeLink *)(node->field[fn]);
				while (nl) {
					if (fn == 0) {
						print_text(text,"\t");
						print_text(text,(char*)(nl->node));
						print_text_newline(text," # ?");
					}
					else {
						print_text(text,"\t");
						print_text(text,fields[fn].fname);
						print_text(text," ");
						print_text_newline(text,(char*)(nl->node));
					};
					nl = nl->next;
				};
				
				break;
				
			case LINK:
				nl = (struct NodeLink *)(node->field[fn]);
				
				while (nl)
				{
					if (nl->node) {
						print_text(text,"\t");
						print_text(text,fields[fn].fname);
						print_text_newline(text,(nl->node->id));
					}
					nl = nl->next;
				}
				
				break;
			}
		
		if (fn == 0)
			break;
	}
	print_text_newline(text,"");
}

void on_select_child(GtkWidget *widget, GtkWidget *child, struct Node *node)
{
	gtk_text_clear(GTK_TEXT(text)); 
	print_node(GTK_TEXT(text),node);
}

void on_select_item(GtkWidget *widget, struct Node *node)
{
	gtk_text_clear(GTK_TEXT(text)); 
	print_node(GTK_TEXT(text),node);
}

void gtk_tree_add_item(GtkTree *tree,gchar *name)
{
	GtkWidget *tree_item;

	tree_item=gtk_tree_item_new_with_label(name);
	gtk_tree_append(GTK_TREE(tree),tree_item);
	gtk_widget_show(tree_item);
}

GtkWidget *gtk_subtree_new(GtkTree *tree,gchar *name)
{
	GtkWidget *tree_item;
	GtkWidget *subtree;

	tree_item=gtk_tree_item_new_with_label(name);
	gtk_tree_append(GTK_TREE(tree),tree_item);
	gtk_widget_show(tree_item);

	subtree=gtk_tree_new();
	gtk_tree_item_set_subtree(GTK_TREE_ITEM(tree_item),subtree);
	return subtree;
}

GtkWidget *gtk_subtree_new_with_signal(GtkTree *tree, struct Node *node)
{
	GtkWidget *tree_item;
	GtkWidget *subtree;

	tree_item=gtk_tree_item_new_with_label(node->name);
	gtk_tree_append(GTK_TREE(tree),tree_item);
	gtk_widget_show(tree_item);

	gtk_signal_connect(GTK_OBJECT(tree_item),"select",GTK_SIGNAL_FUNC(on_select_item),node);

	subtree=gtk_tree_new();
	gtk_tree_item_set_subtree(GTK_TREE_ITEM(tree_item),subtree);
	return subtree;
}

void gtk_tree_add_links(GtkTree *tree, struct Node *node, struct NodeLink *nlink, gchar *name)
{
	GtkWidget *subtree;

	subtree=gtk_subtree_new(tree,name);
	gtk_signal_connect(GTK_OBJECT(subtree),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);
	while (nlink)
	{
		gtk_tree_add_item(GTK_TREE(subtree),nlink->node->name);
		nlink = nlink->next;
	}
}


void gtk_tree_add_node(GtkTree *tree,struct Node* node)
{
	GtkWidget *subtree;
	GtkWidget *tmp;
	struct NodeLink *nlink;

	subtree=gtk_subtree_new_with_signal(GTK_TREE(tree),node);
	gtk_signal_connect(GTK_OBJECT(subtree),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);

	tmp=gtk_subtree_new(GTK_TREE(subtree),"id");
	gtk_tree_add_item(GTK_TREE(tmp),node->id);
	gtk_signal_connect(GTK_OBJECT(tmp),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);

	tmp=gtk_subtree_new(GTK_TREE(subtree),"Type");
	gtk_tree_add_item(GTK_TREE(tmp),node->type);
	gtk_signal_connect(GTK_OBJECT(tmp),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);

        if (node->date) {
		tmp=gtk_subtree_new(GTK_TREE(subtree),"Date");
		gtk_tree_add_item(GTK_TREE(tmp),date_to_str(node->date));
		gtk_signal_connect(GTK_OBJECT(tmp),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);
	}
	if (NULL != (nlink = node->reference)) {
		tmp=gtk_subtree_new(GTK_TREE(subtree),"Reference");
		while (nlink) {
			gtk_tree_add_item(GTK_TREE(tmp),(gchar *)nlink->node);
			nlink = nlink->next;
		}
		gtk_signal_connect(GTK_OBJECT(tmp),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);
	}

        if (node->status) {
		tmp=gtk_subtree_new(GTK_TREE(subtree),"Status");
		gtk_tree_add_item(GTK_TREE(tmp),node->status);
		gtk_signal_connect(GTK_OBJECT(tmp),"select_child",GTK_SIGNAL_FUNC(on_select_child),node);
	}


        if (NULL != (nlink = node->aka)) gtk_tree_add_links(GTK_TREE(subtree),node,nlink,"aka");

        if (NULL != (nlink = node->successorto)) gtk_tree_add_links(GTK_TREE(subtree),node,nlink,"Successor to");

        if (NULL != (nlink = node->codetakenfrom)) gtk_tree_add_links(GTK_TREE(subtree),node,nlink,"Code taken from");

        if (NULL != (nlink = node->influencedby)) gtk_tree_add_links(GTK_TREE(subtree),node,nlink,"Influenced by");

        if (NULL != (nlink = node->runson)) gtk_tree_add_links(GTK_TREE(subtree),node,nlink,"Runs on");
}

void generate_tree(GtkTree *tree, struct Node *nodetree)
{
	if (nodetree!=NULL) {
		gtk_tree_add_node(tree,nodetree);
		generate_tree(tree,nodetree->right);
		generate_tree(tree,nodetree->left);
	}
}



int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *tree;

	GtkWidget *subtree;

	struct Node *nodetree;

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain (PACKAGE);
#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);
	argc--;
	argv++;
	nodetree = parse_files (argc, argv);

	add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
	add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
	window = create_window ();
	tree=lookup_widget(window,"root_tree");
	text=lookup_widget(window,"text1");      
	
	print_node(GTK_TEXT(text),nodetree);
	gtk_text_clear(GTK_TEXT(text));
	subtree=gtk_subtree_new(GTK_TREE(tree),"syntax3");

	generate_tree(GTK_TREE(subtree),nodetree);
	
	gtk_widget_show (window);
	
	gtk_main ();
	return 0;
}
