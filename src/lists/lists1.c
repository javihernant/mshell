#include "lists_fts.h"
#include <stdlib.h>

t_list	*create_lstnode(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	node->content = content;
	node->next = 0;
	return (node);
}

void	lstadd_back(t_list **lst, void *data)
{
	t_list	*last;
	t_list	*new_node;

	new_node = create_lstnode(data);
	last = lstlast(*lst);
	if (!last)
		*lst = new_node;
	else
		last->next = new_node;
}

t_list	*lstlast(t_list *lst)
{
	t_list	*last;

	last = lst;
	if (!lst)
		return (0);
	while (last->next != 0)
	{
		last = last->next;
	}
	return (last);
}


