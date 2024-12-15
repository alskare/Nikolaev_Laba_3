#ifndef GOLFILE_H
#define GOLFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    float price;
    int expired_date;
} Product;

void init_database();
void loadFile(const char* filename);
void saveFile(const char* filename);
void add(Product product);
Product* get_products(int* count);
void freeMem(Product* products, int count);
void deleteFile(int index);
void editFile(int index, const char* new_name, float new_price, int new_expiry_date);
void searchFile(const char* name, float min_price, float max_price, Product** results, int* result_count);
void menu();
void userFile();

#endif 
