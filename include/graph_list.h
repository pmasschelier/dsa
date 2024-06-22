#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "graph_mat.h"
#include "list_ref.h"

/* Graphe représenté par des listes d'adjacence */
typedef struct EDGE_LIST EDGE_LIST;
struct EDGE_LIST {
	long long w;  // Poids de l'arcs
	unsigned p;	  // Cible de l'arc
};

typedef struct GRAPH_LIST GRAPH_LIST;
struct GRAPH_LIST {
	unsigned nb_vert;
	list_ref_t* neighbours;
};

/* \brief Crée un graphe sous forme de listes d'adjacence
 * \param Nombre de sommets du graphe (doit être strictement positif)
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire
 * ont été mises à zéro avec calloc) le graphe doit être libéré avec
 * free_graph_list(g); La fonction renvoie nulle si l'allocation à échoué.
 */
GRAPH_LIST* init_graph_list(unsigned size);

/* \brief Copie d'un graphe sous forme de matrice d'adjacence vers un graphe
 * sous forme de listes d'adjacence \param graph_mat graphe à copier \param
 * graph_list destination de la copie (on appelle dessus free_graph_list puis
 * init_graph_list) \return graph_list
 */
GRAPH_LIST* graph_mat_to_graph_list(GRAPH_MAT* graph_mat,
									GRAPH_LIST** graph_list);

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
void set_edge_list(GRAPH_LIST* g,
				   unsigned int a,
				   unsigned int b,
				   BOOL val,
				   long long weight,
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
int mark_and_examine_traversal_list(GRAPH_LIST* g,
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
int DFS_list(GRAPH_LIST* g, unsigned r, int** tab, int** father);
int DFS_list_recursive(GRAPH_LIST* g, unsigned r, int** tab, int** father);

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
int Dijkstra_list(GRAPH_LIST* g,
				  unsigned r,
				  long long** distance,
				  int** father);

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
int topological_numbering_list(GRAPH_LIST* g, unsigned** num, unsigned** denum);

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
int Bellman_list(GRAPH_LIST* g, unsigned r, long long** distance, int** father);

#endif
