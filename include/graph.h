#ifndef GRAPH_H
#define GRAPH_H

#include "structures.h"

/* Graphe représenté par une matrice d'adjacence */
typedef struct EDGE_MAT EDGE_MAT;
struct EDGE_MAT
{
	int w; // Poids de l'arcs
	BOOL b; // L'arc existe ?
};

typedef struct GRAPH_MAT GRAPH_MAT;
struct GRAPH_MAT 
{
	unsigned nb_vert;
	EDGE_MAT** mat;
};

/* \brief Crée un graphe sous forme de matrice d'adjacence
 * \param Nombre de sommets du graphe
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire ont
 * été mises à zéro avec calloc) le graphe doit être libéré avec free_graph_mat(g);
 * La fonction renvoie nulle si l'allocation à échoué.
 */
GRAPH_MAT* init_graph_mat(unsigned size);

/* \brief Libère un graphe sous forme de matrice d'adjacence */
void free_graph_mat(GRAPH_MAT* g);

/* \brief Crée ou détruit un arc entre deux sommets
 * \param Pointeur vers le graphe
 * \param Sommet source
 * \param Sommet destination
 * \param TRUE : l'arc est créé, FALSE : l'arc est détruit
 * \param TRUE : L'arc est créé dans les deux sens : (a, b) et (b, a)
 */
void set_edge_mat(GRAPH_MAT* g,  unsigned int a, unsigned int b, BOOL val, BOOL reverse);

/* \brief Modifie le poids d'un arc
 * \param Pointeur vers le graphe
 * \param Sommet source
 * \param Sommet destination
 * \param Poids de l'arc
 * \param TRUE : L'arc est créé dans les deux sens : (a, b) et (b, a)
 */
void set_edge_mat_weight(GRAPH_MAT* g,  unsigned int a, unsigned int b, int weight, BOOL reverse);

/* \brief Implémentation de l'algorithme de Dijkstra avec un graphe sous forme de matrice d'adjacence
 * \param Pointeur vers le graphe, !!! Les arêtes du graphe ne doivent avoir que des poids posisitfs.
 * \param Racine du graphe
 * \param pointeur vers un pointeur sur un int
 * \param pointeur vers un pointeur sur un int
 * Ces deux derniers pointeurs vont être modifiés pour pointer vers des tableau alloués
 * de la taille graphe->nb_vert !!! Ils devront être libérés par l'utilisateur !!!
 * \complexity O(n²)
 */
void Dijkstra_mat(GRAPH_MAT* g, unsigned r, int** father, int** distance);


#endif
