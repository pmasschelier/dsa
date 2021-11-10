#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "list.h"
#include "graph_mat.h"

/* Graphe représenté par des listes d'adjacence */
typedef struct EDGE_LIST EDGE_LIST;
struct EDGE_LIST
{
	long long w; // Poids de l'arcs
	unsigned p; // Cible de l'arc
};

typedef struct GRAPH_LIST GRAPH_LIST;
struct GRAPH_LIST
{
	unsigned nb_vert;
	LIST* neighbours;
};

/* \brief Crée un graphe sous forme de listes d'adjacence
 * \param Nombre de sommets du graphe (doit être strictement positif)
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire ont
 * été mises à zéro avec calloc) le graphe doit être libéré avec free_graph_list(g);
 * La fonction renvoie nulle si l'allocation à échoué.
 */
GRAPH_LIST* init_graph_list(unsigned size);

/* \brief Copie d'un graphe sous forme de matrice d'adjacence vers un graphe sous forme de listes d'adjacence
 * \param graph_mat graphe à copier
 * \param graph_list destination de la copie (on appelle dessus free_graph_list puis init_graph_list)
 * \return graph_list
 */
GRAPH_LIST* graph_mat_to_graph_list(GRAPH_MAT* graph_mat, GRAPH_LIST** graph_list);

/* \brief Libère un graphe sous forme de listes d'adjacence */
void free_graph_list(GRAPH_LIST* g);

/* \brief Crée ou détruit un arc entre deux sommets
 * \param Pointeur vers le graphe
 * \param Sommet source
 * \param Sommet destination
 * \param TRUE : l'arc est créé, FALSE : l'arc est détruit
 * \param weight poids de l'arc s'il doit être créé
 * \param TRUE : L'arc est créé dans les deux sens : (a, b) et (b, a)
 */
void set_edge_list(GRAPH_LIST* g,  unsigned int a, unsigned int b, BOOL val, long long weight, BOOL reverse);

#endif
