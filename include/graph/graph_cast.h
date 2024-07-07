#ifndef GRAPHCAST_H
#define GRAPHCAST_H

typedef struct graph_mat graph_mat_t;
struct graph_mat;

typedef struct graph_list graph_list_t;
struct graph_list;

/* \brief Copie d'un graphe sous forme de matrice d'adjacence vers un graphe
 * sous forme de listes d'adjacence \param graph_mat graphe à copier \param
 * graph_list destination de la copie (on appelle dessus free_graph_list puis
 * init_graph_list) \return graph_list
 */
graph_list_t* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									  graph_list_t** graph_list);

#endif	// !GRAPHCAST_H
