#include "golfile.h"

static Product* products = NULL;
static int product_count = 0;

void init_database() {
	products = NULL;
	product_count = 0;
}

void loadFile(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file)
		return;

	fread(&product_count, sizeof(int), 1, file);

	if (product_count > 0) {
		products = malloc(product_count * sizeof(Product));
        for (int i = 0; i < product_count; ++i) {
		Product* p = &products[i];
	size_t len;
	fread(&len, sizeof(size_t), 1, file);
        p->name = malloc(len + 1);
        fread(p->name, sizeof(char), len, file);
        p->name[len] = '\0';
        fread(&p->price, sizeof(float), 1, file);
        fread(&p->expired_date, sizeof(int), 1, file);
        	}
    	}
    	fclose(file);
}

void saveFile(const char* filename) {
	FILE* file = fopen(filename, "wb");
    	if (!file)
		return;
    	fwrite(&product_count, sizeof(int), 1, file);

    	for (int i = 0; i < product_count; ++i) {
        	Product* p = &products[i];

        size_t len = strlen(p->name);
        fwrite(&len, sizeof(size_t), 1, file);
        fwrite(p->name, sizeof(char), len, file);
        fwrite(&p->price, sizeof(float), 1, file);
        fwrite(&p->expired_date, sizeof(int), 1, file);
    	}
    	fclose(file);
}

void add(Product product) {
	products = realloc(products, (product_count + 1) * sizeof(Product));
    	products[product_count++] = product;
}

Product* get_products(int* count) {
    	*count = product_count;
    	return products;
}

void freeMem(Product* products, int count) {
    	for (int i = 0; i < count; ++i) {
        	free(products[i].name);
    	}
    	free(products);
}

void deleteFile(int index) {
    	if (index >= 0 && index < product_count) {
        	for (int i = index; i < product_count - 1; ++i) {
            		products[i] = products[i + 1];
        	}
        product_count--;
        products = realloc(products, product_count * sizeof(Product));
    	}
}

void editFile(int index, const char* new_name, float new_price, int new_expiry_date) {
    	if (index >= 0 && index < product_count) {
        	Product* p = &products[index];
        	free(p->name);
        	p->name = strdup(new_name);
        	p->price = new_price;
        	p->expired_date = new_expiry_date;
    	}
}

void searchFile(const char* name, float min_price, float max_price, Product** results, int* result_count) {
    	*result_count = 0;
    	*results = NULL; 
    	for (int i = 0; i < product_count; ++i) {
        	Product* p = &products[i];
        	if (strstr(p->name, name) != NULL && p->price >= min_price && p->price <= max_price) {
            		(*results) = realloc(*results, ((*result_count) + 1) * sizeof(Product));
            		(*results)[(*result_count)] = *p;
            		(*result_count)++;
        	}
    	}
}

void menu() {
    	printf("1. Tovari\n");
    	printf("2. Add tovar\n");
    	printf("3. Delete tovar\n");
    	printf("4. Izmenit' tovar\n");
    	printf("5. Search tovar\n");
    	printf("6. Soxranenie bazi dannix\n");
    	printf("7. Zagruzka bazi dannix\n");
    	printf("8. EXIT\n");
    	printf("\nVvedite nomer: ");
}

void userFile() {
    	while (1) {
        	menu();
        	int choice;
        	scanf("%d", &choice);

        switch (choice) {
		case 1: {
                	int count;
                	Product* prods = get_products(&count);
                	if (count == 0) {
                    		printf("NO tovari.\n");
                	} else {
                    		for (int i = 0; i < count; ++i) {
                        	Product* p = &prods[i];
                        	printf("Nazvanie: %s  Cena: %.2f  SrokGodnosti: %d\n", p->name, p->price, p->expired_date);
                    	}
                }
                break;
	}
            	case 2: {
                	char name[100];
                	float price;
                	int expired_date;

                	printf("Vvedite nazvanie tovara: ");
                	scanf("%s", name);
                	printf("Vvedite cenu tovara: ");
                	scanf("%f", &price);
                	printf("Vvedite srok godnosti: ");
                	scanf("%d", &expired_date);

                	Product product = { strdup(name), price, expired_date };
                	add(product);
                	break;
            	}
            	case 3: {
                	int count;
                	Product* prods = get_products(&count);

                	if (count == 0) {
                    		printf("Net tovarov.\n");
                	} else {
                    		printf("Viberite index tovara dlya delete'anya:\n");

                    		for (int i = 0; i < count; ++i) {
                        		Product* p = &prods[i];
                        		printf("%d. Nazvanie: %s  Cena: %.2f  Srok godnosti: %d\n", i, p->name, p->price, p->expired_date);
                    		}

                    	int index;
                    	scanf("%d", &index);
                    	deleteFile(index);
                	}
                	break;
            	}
            	case 4: {
                	int count;
                	Product* prods = get_products(&count);

                	if (count == 0) {
                    		printf("Net tovarov.\n");
                	} else {
                    		printf("Index dlya redakta vvedite:\n");

                    		for (int i = 0; i < count; ++i) {
                        		Product* p = &prods[i];
                        		printf("%d. Nazvanie: %s  Cena: %.2f  Srok godnosti: %d\n", i, p->name, p->price, p->expired_date);
                    		}

                    		int index;
                    		scanf("%d", &index);

                    		char new_name[100];
                    		float new_price;
                    		int new_expired_date;

                    		printf("New imya: ");
                    		scanf("%s", new_name);
                    		printf("New cena: ");
                    		scanf("%f", &new_price);
                    		printf("New srok godnosti: ");
                    		scanf("%d", &new_expired_date);

                    		editFile(index, new_name, new_price, new_expired_date);
                		}
                	break;
            		}
            	case 5: {
                	char name[100];
                	float min_price, max_price;

                	printf("Nazvanie tovara: ");
                	scanf("%s", name);
                	printf("Min cena: ");
                	scanf("%f", &min_price);
                	printf("Max cena: ");
                	scanf("%f", &max_price);

                	Product* results;
                	int result_count;
                	searchFile(name, min_price, max_price, &results, &result_count);

                	if (result_count == 0) {
                    		printf("Tovara nety\n");
                	} else {
                    		for (int i = 0; i < result_count; ++i) {
                        	Product* p = &results[i];
                        	printf("Nazvanie: %s  Cena: %.2f  Srok godnosti: %d\n", p->name, p->price, p->expired_date);
                    		}
                	}
                	free(results);
                	break;
            	}
		case 6: {
                	saveFile("database.bin");
                	printf("Baza soxranena\n");
                	break;
            	}
            	case 7: {
                	loadFile("database.bin");
                	printf("Baza zagruzhena.\n");
                	break;
            	}
            	case 8: {
                	freeMem(products, product_count);
                	exit(0);
            	}
            	default:
                	printf("Nepravil'no, poprobuyi eshe raz -_-\n");
        	}
    	}
}
