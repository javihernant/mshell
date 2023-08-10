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

int	lstlen(t_list *lst)
{
	int	i;

	i = 0;
	while (lst != 0)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

//frees node and returns next; It does not free its content
t_list	*next(t_list *curr)
{
	t_list	*n;

	if (curr == 0)
		return 0;
	n = curr->next;
	free(curr);
	return (n);
}

