#ifndef LISTS_FTS_H
# define LISTS_FTS_H

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*create_lstnode(void *content);
void	lstadd_back(t_list **lst, void *data);
t_list	*lstlast(t_list *lst);
t_list	*next(t_list *curr);

#endif