#ifndef GRAPH_MAT_H
#define GRAPH_MAT_H

#include "structures.h"

extern const long long int INFINITY;

/* Graphe représenté par une matrice d'adjacence */
typedef struct EDGE_MAT EDGE_MAT;
struct EDGE_MAT {
	long long w;  // Poids de l'arcs
	BOOL b;		  // L'arc existe ?
};

typedef struct graph_mat graph_mat_t;
struct graph_mat {
	unsigned nb_vert;
	long long* weights;
	BOOL* edges;
	// EDGE_MAT** mat;
};

/* \brief Crée un graphe sous forme de matrice d'adjacence
 * \param Nombre de sommets du graphe (doit être strictement positif)
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire
 * ont été mises à zéro avec calloc) le graphe doit être libéré avec
 * free_graph_mat(g); La fonction renvoie nulle si l'allocation à échoué.
 */
graph_mat_t* create_graph_mat(unsigned size, BOOL has_weights);

/* \brief Libère un graphe sous forme de matrice d'adjacence */
void free_graph_mat(graph_mat_t* g);

/* \brief Crée ou détruit un arc entre deux sommets
 * \param Pointeur vers le graphe
 * \param Sommet source
 * \param Sommet destination
 * \param TRUE : l'arc est créé, FALSE : l'arc est détruit
 * \param TRUE : L'arc est créé dans les deux sens : (a, b) et (b, a)
 */
void graph_mat_set_edge(graph_mat_t* g,
						unsigned int a,
						unsigned int b,
						BOOL val,
						long long weight,
						BOOL reverse);

BOOL graph_mat_get_edge(graph_mat_t* g, unsigned int a, unsigned b);
BOOL graph_mat_get_weight(graph_mat_t* g, unsigned int a, unsigned b);

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
 * d'erreur et sinon le nombre de sommets rencontrés \complexity 0(n²)
 */
int mark_and_examine_traversal_mat(graph_mat_t* g,
								   unsigned r,
								   int** tab,
								   int** father,
								   LIST_STRUCT queue_or_stack);

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
// int DFS_mat(GRAPH_MAT* g, unsigned r, int** tab, int** father);
int graph_mat_preorder_dfs(graph_mat_t* g,
						   unsigned r,
						   int* values,
						   int* father);
int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father);

/* \brief Implémentation de l'algorithme de Dijkstra avec un graphe sous forme
 * de matrice d'adjacence \param Pointeur vers le graphe, !!! Les arêtes du
 * graphe ne doivent avoir que des poids posisitfs. \param Racine du graphe
 * \param après la fonction distance[i] = d(r, i) et INFINI si le sommet n'est
 * pas atteint \param si father != NULL, après la fonction father[i] est le père
 * de i Ces deux derniers pointeurs vont être modifiés pour pointer vers des
 * tableau alloués de la taille graphe->nb_vert !!! Ils devront être libérés par
 * l'utilisateur !!! \return Nombre de sommets atteints par l'algorithme et -1
 * en cas d'échec \complexity O(n²)
 */
int Dijkstra_mat(graph_mat_t* g,
				 unsigned r,
				 long long** distance,
				 int** father);

/* \brief Numérotation topologique du graphe
 * Associe à chaque sommet i d'un graphe orienté acyclique (DAG) un numéro
 * num[i] tel que si j est un ascendant de i (*num)[j] < (*num)[i]
 * \param g Pointeur vers le graphe, !!! Ce doit être un DAG !!!
 * \param après la fonction (*num)[i] est le numéro topologique de i
 * \param si denum != NULL : après la fonction (*denum)[num[i]]=i
 * Ces trois derniers pointeurs vont être modifiés pour pointer vers des tableau
 * alloués de la taille graphe->nb_vert !!! Ils devront être libérés par
 * l'utilisateur !!! \return 0 si la numérotation a bien eu lieu -1 si father ==
 * NULL ou g->nb_vert == 0 -2 s'il y a eu une erreur d'allocation >0 si g
 * n'était pas un DAG \complexity 0(n²)
 */
int topological_numbering_mat(graph_mat_t* g, unsigned** num, unsigned** denum);

/* \brief Implémentation de l'algorithme de Bellman avec un graphe sous forme de
 * matrice d'adjacence \param Pointeur vers le graphe, !!! Ce doit être un DAG
 * !!! \param Racine du graphe \param après la fonction distance[i] = d(r, i) et
 * INFINI si le sommet n'est pas atteint \param si father != NULL, après la
 * fonction father[i] est le père de i Ces deux derniers pointeurs vont être
 * modifiés pour pointer vers des tableau alloués de la taille graphe->nb_vert
 * !!! Ils devront être libérés par l'utilisateur !!! \return Nombre de sommets
 * atteints par l'algorithme et -1 en cas d'échec \complexity O(n²)
 */
int Bellman_mat(graph_mat_t* g, unsigned r, long long** distance, int** father);

// int Ford_and_Fulkerson(GRAPH_MAT* g, unsigned src, unsigned well, )

#endif
