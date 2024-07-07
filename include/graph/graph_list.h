#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "list_ref/list_ref.h"
#include "weight_type.h"

/* Graphe représenté par des listes d'adjacence */
typedef struct graph_list_edge graph_list_edge_t;
struct graph_list_edge {
	graph_weight_t w;  // Poids de l'arcs
	unsigned to;	   // Cible de l'arc
};

typedef struct graph_list graph_list_t;
struct graph_list {
	unsigned nb_vert;
	list_ref_t* neighbours;
	BOOL is_weighted;
};

/* \brief Crée un graphe sous forme de listes d'adjacence
 * \param Nombre de sommets du graphe (doit être strictement positif)
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire
 * ont été mises à zéro avec calloc) le graphe doit être libéré avec
 * free_graph_list(g); La fonction renvoie nulle si l'allocation à échoué.
 */
graph_list_t* create_graph_list(unsigned size, BOOL is_weighted);


/* \brief Libère un graphe sous forme de listes d'adjacence */
void free_graph_list(graph_list_t* g);

BOOL graph_list_get_edge(graph_list_t* g, unsigned int a, unsigned int b);

/* \brief Crée ou détruit un arc entre deux sommets
 * \param Pointeur vers le graphe
 * \param Sommet source
 * \param Sommet destination
 * \param TRUE : l'arc est créé, FALSE : l'arc est détruit
 * \param weight poids de l'arc s'il doit être créé
 * \param TRUE : L'arc est créé dans les deux sens : (a, b) et (b, a)
 */
void graph_list_set_edge(graph_list_t* g,
						 unsigned int a,
						 unsigned int b,
						 BOOL val,
						 graph_weight_t weight,
						 BOOL reverse);

/* \brief Parcours "marquer et examiner" du graphe
 * \param g Pointeur vers le graphe
 * \param r Racine du parcours
 * \param tab Liste des sommets dans l'ordre rencontrés
 * \param father Tableau tel que father[i] soit le père de i si i a été
 * rencontré lors du parcours Ces deux derniers pointeurs vont être modifiés
 * pour pointer vers des tableau alloués de la taille graphe->nb_vert !!! Ils
 * devront être libérés par l'utilisateur !!! \param queue_or_stack Type de la
 * file d'attente, doit valoir QUEUE ou STACK QUEUE : On a un parcours en
 * largeur (BFS) STACK : On a un parcours proche du DFS \return -1 en cas
 * d'erreur et sinon le nombre de sommets rencontrés \complexity 0(n·m)
 */
int mark_and_examine_traversal_list(graph_list_t* g,
									unsigned r,
									int* tab,
									int* father,
									LIST_STRUCT queue_or_stack);

int graph_list_bfs(graph_list_t* g, unsigned r, int* values, int* father);

/* \brief Parcours en profondeur du graphe
 * \param g Pointeur vers le graphe
 * \param r Racine du parcours
 * \param tab Liste des sommets dans l'ordre rencontrés
 * \param father Tableau tel que father[i] soit le père de i si i a été
 * rencontré lors du parcours Ces deux derniers pointeurs vont être modifiés
 * pour pointer vers des tableau alloués de la taille graphe->nb_vert !!! Ils
 * devront être libérés par l'utilisateur !!! \return -1 en cas d'erreur et
 * sinon le nombre de sommets rencontrés
 */
// int DFS_list(graph_list_t* g, unsigned r, int** tab, int** father);
int graph_list_preorder_dfs(graph_list_t* g, unsigned r, int* tab, int* father);

/* \brief Implémentation de l'algorithme de Dijkstra avec un graphe sous forme
 * de listes d'adjacence \param g Pointeur vers le graphe, !!! Les arêtes du
 * graphe ne doivent avoir que des poids posisitfs. \param r Racine du graphe
 * \param distance Après la fonction distance[i] = d(r, i) et INFINI si le
 * sommet n'est pas atteint \param father Si father != NULL, après la fonction
 * father[i] est le père de i Ces deux derniers pointeurs vont être modifiés
 * pour pointer vers des tableau alloués de la taille graphe->nb_vert !!! Ils
 * devront être libérés par l'utilisateur !!! \return Nombre de sommets atteints
 * par l'algorithme et -1 en cas d'échec \complexity O(n²)
 */
int graph_list_dijkstra(graph_list_t* g,
						unsigned r,
						graph_weight_t* distance,
						int* father);

/* \brief Numérotation topologique du graphe
 * Associe à chaque sommet i d'un graphe orienté acyclique (DAG) un numéro
 * num[i] tel que si j est un ascendant de i (*num)[j] < (*num)[i]
 * \param g Pointeur vers le graphe, !!! Ce doit être un DAG !!!
 * \param num Après la fonction (*num)[i] est le numéro topologique de i
 * \param denum si denum != NULL : après la fonction (*denum)[num[i]]=i
 * Ces trois derniers pointeurs vont être modifiés pour pointer vers des tableau
 * alloués de la taille graphe->nb_vert !!! Ils devront être libérés par
 * l'utilisateur !!! \return 0 si la numérotation a bien eu lieu -1 si father ==
 * NULL ou g->nb_vert == 0 -2 s'il y a eu une erreur d'allocation >0 si g
 * n'était pas un DAG \complexity 0(n²)
 */
int topological_numbering_list(graph_list_t* g,
							   unsigned** num,
							   unsigned** denum);

/* \brief Implémentation de l'algorithme de Bellman avec un graphe sous forme de
 * listes d'adjacence \param g Pointeur vers le graphe, !!! Ce doit être un DAG
 * !!! \param r Racine du graphe \param distance Après la fonction distance[i] =
 * d(r, i) et INFINI si le sommet n'est pas atteint \param father Si father !=
 * NULL, après la fonction father[i] est le père de i Ces deux derniers
 * pointeurs vont être modifiés pour pointer vers des tableau alloués de la
 * taille graphe->nb_vert !!! Ils devront être libérés par l'utilisateur !!!
 * \return Nombre de sommets atteints par l'algorithme et -1 en cas d'échec
 * \complexity O(n²)
 */
int Bellman_list(graph_list_t* g,
				 unsigned r,
				 long long** distance,
				 int** father);

#endif
