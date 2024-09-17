
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Material {
char name[100];
double quantity;
char unit[20];
struct Material* next;
} Material;

typedef struct Recipe {
char name[100];
char type[20];
Material* materials;
int  numMaterials;
struct Recipe* next;
    
} Recipe;


typedef struct Node {
Recipe recipe;
struct Node* next;
} Node;

Recipe* recipeList = NULL;
Material* prev = NULL;
Node* head = NULL;

int validateString(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!(isalpha(str[i]) || (str[i] >= '0' && str[i] <= '9'))) {
            return 0;
        }
    }
    return 1;
}

bool isDouble(char* input) {
    char* endptr;
    strtod(input, &endptr);
    if (*endptr != '\0') {
        return false;
    }
    return true;
}



// Function to create a new material node
Material* createMaterial(char name[], double quantity, char unit[]) {
Material* newMaterial = (Material*)malloc(sizeof(Material));
strcpy(newMaterial->name, name);
newMaterial->quantity = quantity;
strcpy(newMaterial->unit, unit);
newMaterial->next = NULL;
return newMaterial;
}

// Function to create a new recipe node
Recipe* createRecipe(char name[], char type[]) {
Recipe* newRecipe = (Recipe*)malloc(sizeof(Recipe));
strcpy(newRecipe->name, name);
strcpy(newRecipe->type, type);
newRecipe->next = NULL;
newRecipe->materials = NULL;
return newRecipe;
}

// Function to insert a recipe at the end of the linked list
void insertRecipe(Recipe* newRecipe) {
    
    if (recipeList == NULL) {
        recipeList = newRecipe;
    } else {
        Recipe* current = recipeList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newRecipe;
    }


}

// Function to search for a specific recipe by name
Recipe* searchRecipe(char name[]) {
    Recipe* current = recipeList;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


/// Function to search for a specific material by name in a recipe

Material* searchMaterial(Material *head, const char *materialName) {
    Material *current = head;
    while (current != NULL) {
        if (strcmp(current->name, materialName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}




// Function to print all the recipes
void displayRecipes( ) {
    Recipe* current = recipeList;
    while (current != NULL) {
        printf("Recipe Name : %s\n", current->name);
        printf("Recipe Type : %s\n", current->type);
        printf("Raw Materials:\n");
        Material* materialCurrent = current->materials;
        while (materialCurrent != NULL) {
            printf("Material Name: %s, Quantity: %.2lf %s\n", materialCurrent->name, materialCurrent->quantity, materialCurrent->unit);
            materialCurrent = materialCurrent->next;
        }
        printf("-----------------------\n");
        current = current->next;
    }
}

// Function to insert a material into a recipe
void insertMaterial(Recipe* recipe, Material* material) {
    if (recipe->materials == NULL) {
        recipe->materials = material;
    } else {
        Material* current = recipe->materials;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = material;
    }
}

void searchAndInsertMaterial( const char *recipeName, const char *searchMaterialName, Material *newMaterial,int position) {
    Recipe* head=recipeList;
    Recipe* current = head;

    while (current != NULL) {
        if (strcmp(current->name, recipeName) == 0) {
            Material* material = current->materials;
            while (material != NULL) {
                if (strcmp(material->name, searchMaterialName) == 0) {
                    if (position == 1) {
                        newMaterial->next = material->next;
                        material->next = newMaterial;
                    } else if(position == 2) {
                        newMaterial->next = material;
                        if (current->materials == material) {
                            current->materials = newMaterial;
                        }
                    }
                    return;
                }
                material = material->next;
            }
        }
        current = current->next;
    }

    printf("Recipe or material not found!\n");
}


// Function to update a specific field of a material in a recipe
void updateMaterialField(Material* material, int field) {
    char newValue[50];
    double newQuantity;
    
    
    printf("Enter the new value for the field: ");
    if (field == 1) {
        scanf("%s", newValue);
        while (!validateString(newValue)) {
            printf("Invalid input! Enter a proper string for the material name: ");
            scanf("%s", newValue);
        }
        strcpy(material->name, newValue);
    }
    else if (field == 2) {
        char quantityStr[20];
        scanf("%s", quantityStr);
        while (!isDouble(quantityStr)) {
            printf("Invalid input! Enter a proper double integer for the quantity: ");
            scanf("%s", quantityStr);
        }
        newQuantity = atof(quantityStr);
        material->quantity = newQuantity;
    }
    else if (field == 3) {
        scanf("%s", newValue);
        while (strcmp(newValue, "liters") != 0 && strcmp(newValue, "kilograms") != 0) {
            printf("Invalid input! Enter either 'liters' or 'kilograms' for the unit: ");
            scanf("%s", newValue);
        }
        strcpy(material->unit, newValue);
    }
    else {
        printf("Invalid field value!");
    }
}

// Function to update a specific field of a material in a recipe
void updateRecipeMaterial(Recipe* recipe, int materialIndex, int field) {
    int counter = 1;
    Material* currentMaterial = recipe->materials;
    while (currentMaterial != NULL && counter < materialIndex) {
        currentMaterial = currentMaterial->next;
        counter++;
    }

    if (currentMaterial != NULL) {
        updateMaterialField(currentMaterial, field);
    }
    else {
        printf("Material index not found!\n");
    }
}





// Function to delete a recipe
void deleteRecipe(char name[]) {
    Recipe* current = searchRecipe(name);;
    Recipe* prev = NULL;
    char choice;
while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Are you sure you want to delete \"%s\"? (y/n): ", name);
            scanf(" %c", &choice);

            if (choice == 'y' || choice == 'Y') {
                if (prev == NULL) {
                    recipeList = current->next;
                } else {
                    prev->next = current->next;
                } 
            } else {
                printf("Deletion canceled!\n");
            }
            free(current);
            printf("Deleted\n");
            return;
            
        }
        prev = current;
        current = current->next;
    }

    printf("Recipe \"%s\" not found!\n", name);
}

// Function to write all recipes to a text file
void writeRecipesToFile() {
    FILE* file = fopen("recipes.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Recipe* current = recipeList;
    while (current != NULL) {
        fprintf(file, "Name: %s\n", current->name);
        fprintf(file, "Type: %s\n", current->type);
        Material* water = current->materials;
        while (water != NULL) {
            fprintf(file, "Material: %s\n", water->name);
            fprintf(file, "Quantity: %.2f %s\n", water->quantity, water->unit);
            water = water->next;
        }
        
        fprintf(file, "okay\n");
        current = current->next;
        
    }

    fclose(file);
    
}



// Function to sort materials in a recipe using merge sort algorithm
// Function to sort raw material names using merge sort
Material* merge(Material* left, Material* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    Material* result = NULL;
    
    if (strcmp(left->name, right->name) <= 0) {
        result = left;
        result->next = merge(left->next,right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

void split(Material* source, Material** left, Material** right) {
    Material* slow = source;
    Material* fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *left = source;
    *right = slow->next;
    slow->next = NULL;
}

void mergeSort(Material** headRef) {
    Material* head = *headRef;
    Material* left;
    Material* right;

    if (head == NULL || head->next == NULL) {
        return;
    }

    split(head, &left, &right);
    mergeSort(&left);
    mergeSort(&right);
    *headRef = merge(left, right);
}


int partition(Material *arr[], int low, int high) {
    Material *pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j]->name, pivot->name) < 0) {
            i++;
            Material *temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    Material *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(Material *arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void sortMaterialsInRecipe(char recipeName[50]) {
    
    Recipe *temp = recipeList;
    while (temp != NULL) {
        if (strcmp(temp->name, recipeName) == 0) {
            int count = 0;
            Material *mat = temp->materials;
            while (mat != NULL) {
                count++;
                mat = mat->next;
            }

            Material **arr = malloc(count * sizeof(Material *));
            mat = temp->materials;
            int i = 0;
            while (mat != NULL) {
                arr[i] = mat;
                mat = mat->next;
                i++;
            }

            quickSort(arr, 0, count - 1);

            temp->materials = arr[0];
            mat = temp->materials;
            for (int j = 1; j < count; j++) {
                mat->next = arr[j];
                mat = mat->next;
            }
            mat->next = NULL;

            free(arr);
            printf("Materials in recipe '%s' sorted successfully!\n", recipeName);
            return;
        }
        temp = temp->next;
    }

    printf("Recipe with name '%s' not found!\n", recipeName);
}











int main() {
    int choice;
    char recipeName[100];
    char recipeType[20];
    char materialName[100];
    char newName[100];
    char quantityInput[20];
    double quantity;
    char unit[20];
    Recipe* recipe ;
    Material* material;
    Recipe* recipeList = NULL ;
    struct Material* head = NULL;
    

    do {
        printf("\n=== Recipe Management System ===\n");
        printf("1. Insert Recipe\n");
        printf("2. Add material\n");
        printf("3. Display Recipes\n");
        printf("4. Sorting Methods\n");
        printf("5. Update Recipe \n");
        printf("6. Delete Recipe \n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            do{
                
                
                printf("Enter Recipe Name: ");
                scanf(" %s[^\n]", recipeName);
                recipeName[strcspn(recipeName, "\n")] = '\0';
                while (!validateString(recipeName)) {
                printf("Invalid input!\n Enter a proper string for recipe name: ");
                scanf("%s", recipeName);
                }
            
                printf("Enter recipe type (Standard/Advanced): ");
                scanf(" %s[^\n]", recipeType);
                recipeType[strcspn(recipeType, "\n")] = '\0';
                while (strcmp(recipeType, "Standard") != 0 && strcmp(recipeType, "Advanced") != 0) {
                printf("Invalid input! \n Enter recipe type (Standard/Advanced): ");
                scanf(" %s[^\n]", recipeType);
                recipeType[strcspn(recipeType, "\n")] = '\0';
                }

                recipe = createRecipe(recipeName, recipeType);
                insertRecipe(recipe);

                printf("Enter the number of materials: ");
                int numMaterials;
                scanf("%d", &numMaterials);
                for (int i = 0; i < numMaterials; i++) {
                    printf("Enter Material Name: ");
                    scanf(" %s[^\n]", materialName);
                    materialName[strcspn(materialName, "\n")] = '\0';
                while (!validateString(materialName)) {
                printf("Invalid input!\n Enter Material Name: ");
                scanf("%s", materialName);
                }

        printf("Enter quantity: ");
        scanf("%s", quantityInput);
        quantityInput[strcspn(quantityInput, "\n")] = '\0';

        while (!isDouble(quantityInput)) {
            printf("Invalid input!\n Enter quantity: ");
            scanf("%s", quantityInput);
            quantityInput[strcspn(quantityInput, "\n")] = '\0';
        }
        quantity = atof(quantityInput);


                    printf("Enter Unit (liters/kilograms):  ");
                    scanf(" %s[^\n]", unit);
                    unit[strcspn(unit, "\n")] = '\0';
                    while (strcmp(unit, "liters") != 0 && strcmp(unit, "kilograms") != 0) {
                    printf("Invalid input! \n Enter Unit (liters/kilograms): ");
                    scanf(" %s[^\n]", unit);
                    unit[strcspn(unit, "\n")] = '\0';
                    }

                    material = createMaterial(materialName, quantity, unit);
                    insertMaterial(recipe, material);
                }
                printf("Do you want to add another recipe? (y/n): ");
                scanf(" %c", &choice);
                }while (choice == 'y' || choice == 'Y');
                writeRecipesToFile();
                break;
            
            case 2:
                // Search and Insert Material
                printf("Enter recipe name: ");
                scanf(" %s[^\n]", recipeName);

                printf("Enter material name in recipe you like to search to place new material before or after: ");
                scanf(" %s[^\n]", materialName);

                Recipe *r = searchRecipe(recipeName);
                if (r == NULL) {
                    printf("Recipe not found.\n");
                    break;
                }
                Material *existingMaterial = searchMaterial(recipe->materials, materialName);
                if (existingMaterial == NULL) {
                    printf("Material not found in the recipe.\n");
                    break;
                }
                
                printf("Enter new material name: ");
                char newMaterialName[50];
                scanf(" %s[^\n]", newMaterialName);
                newMaterialName[strcspn(newMaterialName, "\n")] = '\0';
                while (!validateString(newMaterialName)) {
                printf("Invalid input!\nEnter new material Name: ");
                scanf("%s", newMaterialName);
                }




                printf("Enter new material quantity: ");
                double newQuantity;
                scanf("%s", quantityInput);
                quantityInput[strcspn(quantityInput, "\n")] = '\0';

                while (!isDouble(quantityInput)) {
                printf("Invalid input!\nEnter quantity: ");
                scanf("%s", quantityInput);
                quantityInput[strcspn(quantityInput, "\n")] = '\0';
                }
                newQuantity = atof(quantityInput);

                printf("Enter new material unit(liters or kilograms): ");
                char newUnit[10];
                scanf(" %s[^\n]", newUnit);
                newUnit[strcspn(newUnit, "\n")] = '\0';
                    while (strcmp(newUnit, "liters") != 0 && strcmp(newUnit, "kilograms") != 0) {
                    printf("Invalid input! \nEnter Unit (liters/kilograms): ");
                    scanf(" %s[^\n]", newUnit);
                    newUnit[strcspn(newUnit, "\n")] = '\0';
                    }
                
                Material *newMaterial = createMaterial(newMaterialName, newQuantity, newUnit);
                printf("1. Place after\n");
                printf("2. Place before\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                searchAndInsertMaterial(recipeName, materialName, newMaterial,choice);
                printf("Material inserted successfully!\n");
                
                
                
                writeRecipesToFile();
                break;

            case 3:
                displayRecipes();
                break;

            case 4:
            int sort_choice;
            
            printf("Enter recipe name you would like to sort: ");
            scanf(" %s[^\n] ",&recipeName);
            Recipe* sResult = searchRecipe(recipeName);
            if (sResult != NULL) {
                printf("1. Merge Sort\n");
                printf("2. Quick Sort\n");
                printf("Enter choice: ");
                scanf("%d", &sort_choice);
                if (sort_choice == 1) {
                mergeSort(&(recipe->materials));
                writeRecipesToFile();
                }else if (sort_choice == 2) {
                sortMaterialsInRecipe(recipeName);
                writeRecipesToFile();
                } else {
                    printf("Invalid choice.\n");
                }
            
            }
            if (sResult == NULL) {
                    printf("Recipe not found.\n");
                }
                break;
        
            

            case 5:
            int materialIndex, field;

            printf("Enter recipe name you would like to update: ");
            scanf(" %s[^\n] ",&recipeName);
            Recipe* sUpdate = searchRecipe(recipeName);
            if (sUpdate != NULL) {
                Material *existMaterial = searchMaterial(recipe->materials, materialName);
                if (existMaterial == NULL) {
                    printf("Material not found in the recipe.\n");
                    break;
            }
            printf("Enter the index of the material to update: ");
            scanf("%d", &materialIndex);
            printf("Enter the field to update [1: name, 2: quantity, 3: unit]: ");
            scanf("%d", &field);
            updateRecipeMaterial(recipe, materialIndex, field);
            writeRecipesToFile();
            }
            /*printf("Enter recipe name you would like to update: ");
            scanf(" %s[^\n] ",&recipeName);
            Recipe* searchResult = searchRecipe(recipeName);
            if (searchResult != NULL) {
                updateRecipe(recipeName);
                writeRecipesToFile();
            }
            */
                break;
            

            case 6:
        
            printf("Enter recipe name you would like to delete: ");
            scanf(" %s[^\n] ",&recipeName);
            Recipe* deleteResult = searchRecipe(recipeName);
            if (deleteResult != NULL) {
            deleteRecipe(recipeName);
            writeRecipesToFile();
            }
            break;

            case 7:
            printf("Exiting...\n");
            break;

            default:
                printf("Invalid choice!\n");
        }

        printf("\n");
    } while (choice != 7);

    return 0;
}
