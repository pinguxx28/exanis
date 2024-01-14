#ifndef ARMOR_H
#define ARMOR_H

typedef enum {
	NONE,
	MAIL,
	ARMOR_TYPE_N
} armor_type;

typedef struct {
	char *name;
	int protection;
	int durability;
	armor_type type;
} armor_t;

armor_t make_armor(armor_type type);

#endif /* ARMOR_H */
