#ifndef STRUCT_PTR_H
#define STRUCT_PTR_H

enum type { TYPE_INT, TYPE_DOUBLE };

/** @brief Crée un pointeur qui copie la premiere donnée envoyé
 * @param TYPE_INT pour une donnée sur 4 octets et TYPE_DOUBLE pour une donnée
 * sur 8 octets
 * @param ... Le premier paramètre de la liste sera la donnée à copier
 * @return Pointeur vers l'adresse allouée !!! Le pointeur retourné doit être
 * libéré avec free !!!
 */
void* ptr(enum type t, ...);

#endif
