#ifndef STRUCT_PTR_H
#define STRUCT_PTR_H

typedef void (*free_element_fn_t)(void*);

enum type { TYPE_INT, TYPE_DOUBLE };

/** @brief Crée un pointeur qui copie la premiere donnée envoyé
 * @param TYPE_INT pour une donnée sur 4 octets et TYPE_DOUBLE pour une donnée
 * sur 8 octets
 * @param ... Le premier paramètre de la liste sera la donnée à copier
 * @return Pointeur vers l'adresse allouée !!! Le pointeur retourné doit être
 * libéré avec free !!!
 */
void* ptr(enum type t, ...);

static inline void swap_ref(void** refs, int i, int j) {
	void* a = refs[i];
	refs[i] = refs[j];
	refs[j] = a;
}

#endif
