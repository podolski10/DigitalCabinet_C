#include"variable.h"
#include <stdio.h>
#include <conio.h> // Utilisé pour clrscr(), gotoxy(), getch(), etc.
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include<time.h>

//docteur fonctions
int existeedv(const char *filename, int j, int m, int a, int h, int min) {
    FILE *ppt = fopen(filename, "r");
    if (ppt == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    rdv tmp;
    char buffer[256];
    int existe = 0;

    // Utilisation de fgets et sscanf pour une lecture plus robuste
    while (fgets(buffer, sizeof(buffer), ppt) != NULL) {
        int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%d:%d",
                        tmp.patient.nom,
                        tmp.patient.prenom,
                        tmp.patient.cin,
                        tmp.patient.tel,
                        &tmp.jour, &tmp.mois, &tmp.annee,
                        &tmp.heure, &tmp.minute);

        if (res == 9) {
            if (tmp.jour == j && tmp.mois == m &&
                tmp.annee == a && tmp.heure == h &&
                tmp.minute == min) {
                existe = 1;
                break;
            }
        }
    }

    fclose(ppt);
    return existe;
}
void effacerZone(int x, int y, int largeur) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {(SHORT)x, (SHORT)y};


    // Écrire des espaces pour effacer la zone
    for (int i = 0; i < largeur; i++) {
        SetConsoleCursorPosition(hConsole, coord);
        printf(" ");  // Remplace le caractère par un espace
        coord.X++;    // Passe au caractère suivant
    }

    // Réinitialiser la position du curseur au début de la ligne
    coord.X = (SHORT)x;
    SetConsoleCursorPosition(hConsole, coord);
}
void Connecter_Doc() {
    char id[20];
    system("cls");
    afficherTitre("Connexion Docteur");
    gotoxy(30,8);
    textcolor(11);
    printf("Entrez votre ID/CIN : ");
    scanf("%s", id);
    getchar();
    if (!Doc_Existe(id)) {
        system("cls");
        afficherTitre("Connexion Docteur ERROR");
        gotoxy(30,8);
        textcolor(11);
        printf("Aucun docteur trouver avec cet ID. Veuillez vous inscrire d'abord.");
        Sleep(3500);
        system("cls");
        afficherDocteurMenu();
        return;
    }

    afficherTitre("Connexion reussie !");
    Sleep(3000);
    DocMenu();
}
void adddoc() {
    Docteur docteur;
    fflush(stdin);
    FILE *doc = fopen("docteur.txt", "a+");  // Ouvrir le fichier en mode ajout
    if (doc == NULL) {
        perror("Erreur lors de l'ouverture du fichier docteur.txt");
        exit(1);  // Sortir si le fichier ne peut pas être ouvert
    }

    afficherTitre("Inscription Docteur");

    // Entrer le nom du docteur (en autorisant les espaces)
    gotoxy(30, 8);
    textcolor(11);
    printf("Entrez votre nom : ");
    textcolor(7);
    fgets(docteur.nom, sizeof(docteur.nom), stdin);
    docteur.nom[strcspn(docteur.nom, "\n")] = 0;  // Enlever le retour à la ligne

    // Entrer le prénom du docteur (en autorisant les espaces)
    fflush(stdin);
    gotoxy(30, 9);
    textcolor(11);
    printf("Entrez votre prenom : ");
    textcolor(7);
    fgets(docteur.prenom, sizeof(docteur.prenom), stdin);
    docteur.prenom[strcspn(docteur.prenom, "\n")] = 0;  // Enlever le retour à la ligne
    fflush(stdin);
    // Entrer l'ID du docteur
    gotoxy(30, 10);
    textcolor(11);
    printf("Entrez votre ID : ");
    textcolor(7);
    fgets(docteur.id, sizeof(docteur.id), stdin);
    docteur.id[strcspn(docteur.id, "\n")] = 0;
    fflush(stdin);

    // Vérifier si le docteur existe déjà
    if (Doc_Existe(docteur.id)) {
        system("cls");
        afficherTitre("Erreur d\'inscription");
        gotoxy(30, 8);
        textcolor(11);
        printf("Ce docteur existe deja.\n");
        Sleep(4000);
        afficherDocteurMenu();  // Rediriger vers le menu docteur
    } else {
        // Ajouter le docteur au fichier
        fprintf(doc, "%s\t%s\t%s\n", docteur.id, docteur.prenom, docteur.nom);  // Ajouter les informations
        fflush(doc);  // Forcer l'écriture dans le fichier
        fclose(doc);  // Fermer le fichier

        system("cls");
        afficherTitre("Inscription reussie");
        gotoxy(30, 8);
        textcolor(11);
        printf("Vous serez diriger vers le menu du docteur...");
        Sleep(5000);  // Attendre 5 secondes pour que l'utilisateur voie le message
        system("cls");
        DocMenu();  // Rediriger vers le menu du docteur
    }
}



int Doc_Existe(char id[]) {
    doc = fopen("docteur.txt", "r");
    char buffer[256];
    if (doc == NULL) {
        printf("\nImpossible d\'ouvrir le fichier des docteurs.\n");
        return 0;
    }
    Docteur docteur;
    while (fgets(buffer, sizeof(buffer), doc) != NULL)
    {
        // Utilisation de sscanf pour extraire les champs, avec gestion des espaces
        int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%[^\t]\n",docteur.id,docteur.prenom,docteur.nom);
        if (res == 3)
        {
            if (strcmp(docteur.id,id) == 0)
            {
                return 1;
            }
        }
    }

    fclose(doc);
    return 0;
}

//patient functions
// Fonction pour vérifier si une année est bissextile
int estBissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

// Fonction pour obtenir le jour de la semaine pour une date donnée
// 1/1/2024 correspond à un lundi
int jourDeLaSemaine(int jour, int mois, int annee) {
    int joursParMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (estBissextile(annee)) {
        joursParMois[2] = 29;
    }

    int totalJours = 0;
    for (int i = 2024; i < annee; i++) {
        totalJours += estBissextile(i) ? 366 : 365;
    }

    for (int i = 1; i < mois; i++) {
        totalJours += joursParMois[i];
    }

    totalJours += jour - 1;

    return (totalJours + 1) % 7; // 1/1/2024 est un lundi, donc on ajoute 1
}

int estHeureValide(int *heure, int *minute) {
    return (*heure >= 9 && *heure < 18) && (*minute >= 0 && *minute < 60);
}


#include <time.h>

void addRendezvous() {
    fflush(stdin);

    int result;
    afficherTitre("Ajouter un rendez-vous");
    // Saisie des informations du patient
    gotoxy(30, 8); textcolor(11);
    printf("Entrez le nom du patient : ");
    gotoxy(30, 9); textcolor(7);
    fflush(stdin);
    fgets(p.patient.nom, sizeof(p.patient.nom), stdin);
    p.patient.nom[strcspn(p.patient.nom, "\n")] = 0;
    fflush(stdin);

    gotoxy(30, 10); textcolor(11);
    printf("Entrez le prenom : ");
    gotoxy(30, 11); textcolor(7);
    fgets(p.patient.prenom, sizeof(p.patient.prenom), stdin);
    p.patient.prenom[strcspn(p.patient.prenom, "\n")] = 0;
    fflush(stdin);
    gotoxy(30, 12); textcolor(11);
    printf("Entrez le CIN : ");
    gotoxy(30, 13); textcolor(7);
    fgets(p.patient.cin, sizeof(p.patient.cin), stdin);
    p.patient.cin[strcspn(p.patient.cin, "\n")] = 0;
    fflush(stdin);

    gotoxy(30, 14); textcolor(11);
    printf("Entrez le numero de telephone : ");
    gotoxy(30, 15); textcolor(7);
    fgets(p.patient.tel, sizeof(p.patient.tel), stdin);
    p.patient.tel[strcspn(p.patient.tel, "\n")] = 0;
    fflush(stdin);

    // Boucle pour permettre plusieurs tentatives de saisie de la date
again:
    effacerZone(30, 16, 80);
    effacerZone(30, 17, 80);
    pt =fopen(filepatient,"a+");
     if (pt == NULL) {
        printf("Erreur lors de l\'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }
    // Saisie de la date
    gotoxy(30, 16); textcolor(11);
    printf("Saisir la date (jj/mm/aaaa) : ");
    gotoxy(30, 17); textcolor(7);
    result = scanf("%d/%d/%d", &p.jour, &p.mois, &p.annee);
    //printf(">>> after scanning %d , %d , %d \n", p.jour, p.mois, p.annee);
    // Validation du format de la date
    if (result != 3 || p.jour < 1 || p.jour > 31 || p.mois < 1 || p.mois > 12 || p.annee < 1900) {
        effacerZone(30, 16, 80);
        effacerZone(30, 17, 80);
        gotoxy(30, 18); textcolor(4);
        printf("Format de date invalide ou hors limite. Reessayez.\n");
        effacerZone(30,17,80);
        Sleep(2000);
        goto again;
    }

    // Validation que ce n'est pas un dimanche
    if (jourDeLaSemaine(p.jour, p.mois, p.annee) == 0) {
        effacerZone(30, 16, 80);
        effacerZone(30, 17, 80);
        gotoxy(30, 19); textcolor(4);
        printf("La date saisie est un dimanche. Reessayez.\n");
        Sleep(2500);
        effacerZone(30, 19, 80); // Efface le message d'erreur
        Sleep(2000);
        goto again;
    }

    // Nouvelle contrainte : vérifier que la date n'est pas antérieure à la date actuelle
    time_t t = time(NULL);
    struct tm* date_actuelle = localtime(&t);

    // Comparer la date saisie avec la date actuelle
  if (p.annee < (1900 + date_actuelle->tm_year) ||
    (p.annee == (1900 + date_actuelle->tm_year) && p.mois < (date_actuelle->tm_mon + 1)) ||
    (p.annee == (1900 + date_actuelle->tm_year) && p.mois == (date_actuelle->tm_mon + 1) && p.jour < date_actuelle->tm_mday)) {
    effacerZone(30, 16, 80);
    effacerZone(30, 17, 80);
    gotoxy(30, 16); textcolor(4);
    printf("La date saisie est anterieure a la date actuelle. Reessayez.\n");
    Sleep(2000);
    goto again; // Retour à la saisie complète
}
    // Vérification de la contrainte du nombre de rendez-vous pour ce jour
    if (siplein(p.jour, p.mois, p.annee)==0) {
        effacerZone(30, 16, 80);
        effacerZone(30, 17, 80);
        gotoxy(30, 16); textcolor(4);
        printf("Le nombre de rendez-vous pour cette date depasse la limite autorisee.\n");
        effacerZone(30,17,80);
        Sleep(2000);
        goto again; // Retour à la saisie complète
    }

    // Saisie et validation de l'heure
    while (1) {
        // Effacer les zones précédentes pour l'heure
        effacerZone(30, 20, 50);
        effacerZone(30, 21, 50);

        // Saisie de l'heure
        gotoxy(30, 20); textcolor(11);
        printf("Entrez l'heure (hh:mm) : ");
        gotoxy(30, 21); textcolor(7);
        gg:
        result = scanf("%d:%d", &p.heure, &p.minute);

        // Validation du format de l'heure
        if (result != 2 || p.heure < 0 || p.heure > 23 || p.minute < 0 || p.minute >= 60) {
            effacerZone(30, 20, 50);
            effacerZone(30, 21, 50);
            gotoxy(30, 20); textcolor(4);
            printf("Format de l'heure invalide. Reessayez.\n");
            Sleep(2000);
            goto gg;
        }

        // Validation de l'heure dans la plage autorisée
        if (p.heure < 9 || p.heure > 17 || (p.heure == 17 && p.minute > 59)) {
            effacerZone(30, 20, 50);
            effacerZone(30, 21, 50);
            gotoxy(30, 20); textcolor(4);
            printf("L\'heure doit etre entre 9h00 et 17h59. Reessayez.\n");
            Sleep(2000);
            continue;
        }

        break; // Heure valide
    }

    //printf(">>> before exsit dev %d , %d , %d , %d , %d \n", p.jour, p.mois, p.annee, p.heure, p.minute);
    // Vérification des doublons (date et heure)
    if(pt != NULL){
    fclose(pt);
    }
    if (existeedv(filepatient,p.jour,p.mois,p.annee,p.heure,p.minute)==1) {
        effacerZone(30, 16, 80); // Efface la saisie de la date
        effacerZone(30, 17, 80); // Efface le message sous la date
        effacerZone(30, 20, 80); // Efface la saisie de l'heure
        effacerZone(30, 21, 80); // Efface le message sous l'heure

        // Afficher le message d'erreur
        gotoxy(30, 20); textcolor(4);
        printf("Le rendez-vous existe deja. Reessayez.\n");

        Sleep(2000); // Pause pour lire le message

        effacerZone(30, 20, 80); // Efface le message d'erreur

        goto again; // Retour à la saisie complète (date et heure)
    }
    pt = fopen(filepatient, "a+");
    if (pt == NULL) {
        gotoxy(30, 20); textcolor(4);
        printf("Erreur lors de l'ouverture du fichier.\n");
        Sleep(2000);
        return;
    }

    // Ajouter le rendez-vous dans le fichier texte
    fprintf(pt, "%s\t%s\t%s\t%s\t%02d/%02d/%d\t%02d:%02d\n",
            p.patient.nom, p.patient.prenom, p.patient.cin, p.patient.tel,
            p.jour, p.mois, p.annee, p.heure, p.minute);
            fflush(pt);
            fclose(pt);

    // Confirmation de l'ajout
    system("cls");
    afficherTitre("Rendez-vous ajoute avec succes.");
    Sleep(4000);   fclose(pt);
}


// Secretary Functions
int authentifier(char id[])
{
    Docteur d;
    doc = fopen("docteur.txt", "r");
    if (doc == NULL) {
        fprintf(stderr, "NO file existing!\n");
        return 0;
    }
    do
    {
        fscanf(doc," %s %s %s " ,d.id,d.prenom,d.nom);
        if(strcmp(d.id , id) == 0)
    {
            rewind(doc);
            fclose(doc);
            return 1;
    }
    }while(!feof(doc));


    fclose(doc);
    return 0;
}
int occrdv(int j, int m, int a) {
    pt = fopen("patient.txt", "r");
    char buffer[256];
    if (pt == NULL) {
        textcolor(4);
        fprintf(stderr, "Erreur: Impossible d\'ouvrir le fichier.");
        Sleep(3000);
        menu();
    }
    rdv p_temp;
    int cmpt = 0;

     while (fgets(buffer, sizeof(buffer), pt) != NULL) {
        // Utilisation de sscanf pour extraire les champs, avec gestion des espaces
        int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%19s\t%19s\t%d/%d/%d\t%d:%d\n",
                          p_temp.patient.nom, p_temp.patient.prenom, p_temp.patient.cin, p_temp.patient.tel,
                          &p_temp.jour, &p_temp.mois, &p_temp.annee, &p_temp.heure, &p_temp.minute);

        // Vérifier si les données sont valides
        if (res == 9)
        {
        if (j == p_temp.jour && m == p_temp.mois && a == p_temp.annee) {
            cmpt++;
        }
        }
     }
    fclose(pt);
    return cmpt;
}

int siplein(int j , int m ,int a )
{
return occrdv(j,m,a) <= 20 ;
}
void modifierpatient() {
    system("cls");
    rdv tp;  // Nouveau patient modifié
    FILE *telo;
    char id[20];
    char buffer[256];
    int modified = 0;
    char temp_input[256];

    // Ouvrir le fichier source en mode lecture et le fichier temporaire en mode écriture
    pt = fopen(filepatient, "r");
    telo = fopen("temp.txt", "w");

    if (pt == NULL || telo == NULL) {
        gotoxy(30, 8);
        textcolor(4);
        printf("Erreur d'ouverture des fichiers.\n");
        Sleep(2000);
        if (pt) fclose(pt);
        if (telo) fclose(telo);
        afficherMenuSecretaire();
        return;
    }

    afficherTitre("Modifier un patient");
    gotoxy(30, 9);
    textcolor(7);
    printf("Donner l'ID a chercher : ");
    gotoxy(30, 10);
    textcolor(7);
    fflush(stdin);
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    while (fgets(buffer, sizeof(buffer), pt) != NULL) {
        int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%19s\t%19s\t%d/%d/%d\t%d:%d",
                          p.patient.nom, p.patient.prenom, p.patient.cin, p.patient.tel,
                          &p.jour, &p.mois, &p.annee, &p.heure, &p.minute);

        if (res == 9) {
            if (strcmp(p.patient.cin, id) == 0) {
                strcpy(tp.patient.cin, id);

                // Modification du patient
                system("cls");
                afficherTitre("Modification des donnees");

                // Nom
                gotoxy(30, 9);
                textcolor(11);
                printf("Saisir le nouveau nom : ");
                gotoxy(30, 10);
                textcolor(7);
                fflush(stdin);
                fgets(tp.patient.nom, sizeof(tp.patient.nom), stdin);
                tp.patient.nom[strcspn(tp.patient.nom, "\n")] = 0;

                // Prénom
                gotoxy(30, 11);
                textcolor(11);
                printf("Saisir le nouveau prenom : ");
                gotoxy(30, 12);
                textcolor(7);
                fflush(stdin);
                fgets(tp.patient.prenom, sizeof(tp.patient.prenom), stdin);
                tp.patient.prenom[strcspn(tp.patient.prenom, "\n")] = 0;

                // Téléphone
                gotoxy(30, 13);
                textcolor(11);
                printf("Saisir le nouveau numero : ");
                gotoxy(30, 14);
                textcolor(7);
                fflush(stdin);
                fgets(tp.patient.tel, sizeof(tp.patient.tel), stdin);
                tp.patient.tel[strcspn(tp.patient.tel, "\n")] = 0;

                // Date et heure avec validation
                do {
                    gotoxy(30, 15);
                    textcolor(11);
                    printf("Saisir le jour (1-31) : ");
                    gotoxy(30, 16);
                    textcolor(7);
                    fflush(stdin);
                    fgets(temp_input, sizeof(temp_input), stdin);
                    tp.jour = atoi(temp_input);
                } while (tp.jour < 1 || tp.jour > 31);

                do {
                    gotoxy(30, 17);
                    textcolor(11);
                    printf("Saisir le mois (1-12) : ");
                    gotoxy(30, 18);
                    textcolor(7);
                    fflush(stdin);
                    fgets(temp_input, sizeof(temp_input), stdin);
                    tp.mois = atoi(temp_input);
                } while (tp.mois < 1 || tp.mois > 12);

                do {
                    gotoxy(30, 19);
                    textcolor(11);
                    printf("Saisir l'annee (>= 2024) : ");
                    gotoxy(30, 20);
                    textcolor(7);
                    fflush(stdin);
                    fgets(temp_input, sizeof(temp_input), stdin);
                    tp.annee = atoi(temp_input);
                } while (tp.annee < 2024);

                do {
                    gotoxy(30, 21);
                    textcolor(11);
                    printf("Saisir l'heure (9-18) : ");
                    gotoxy(30, 22);
                    textcolor(7);
                    fflush(stdin);
                    fgets(temp_input, sizeof(temp_input), stdin);
                    fflush(stdin);
                    tp.heure = atoi(temp_input);
                } while (tp.heure < 9 || tp.heure > 18);

                do {
                    fflush(stdin);
                    gotoxy(30, 23);
                    textcolor(11);
                    printf("Saisir minute (0-59) : ");
                    gotoxy(30, 24);
                    textcolor(7);
                    fflush(stdin);
                    fgets(temp_input, sizeof(temp_input), stdin);
                    tp.minute = atoi(temp_input);
                } while (tp.minute < 0 || tp.minute > 59);

                fprintf(telo, "%s\t%s\t%s\t%s\t%02d/%02d/%02d\t%02d:%02d\n",
                        tp.patient.nom, tp.patient.prenom, tp.patient.cin, tp.patient.tel,
                        tp.jour, tp.mois, tp.annee, tp.heure, tp.minute);

                modified = 1;
            } else {
                fprintf(telo, "%s\t%s\t%s\t%s\t%02d/%02d/%02d\t%02d:%02d\n",
                        p.patient.nom, p.patient.prenom, p.patient.cin, p.patient.tel,
                        p.jour, p.mois, p.annee, p.heure, p.minute);
            }
        }
    }

    fclose(pt);
    fclose(telo);

    if (modified) {
        if (remove(filepatient) == 0 && rename("temp.txt", filepatient) == 0) {
            system("cls");
            afficherTitre("Modification reussie");
            Sleep(2000);
        } else {
            system("cls");
            gotoxy(30, 9);
            textcolor(4);
            printf("Erreur lors de la mise a jour du fichier");
            Sleep(2000);
        }
    } else {
        system("cls");
        gotoxy(30, 9);
        textcolor(4);
        printf("Patient non trouve");
        Sleep(2000);
    }

    afficherMenuSecretaire();
}


void supprimerrdv() {
    char id[20];
    int trouve = 0;
    FILE *pt = NULL;
    FILE *cache = NULL;
    FILE *temp = NULL;

    do {
        system("cls");
        afficherTitre("Supprimer un patient");
        gotoxy(30, 8);
        textcolor(12);
        printf("Saisir l'ID pour supprimer : ");
        textcolor(7);
        gotoxy(30, 9);
        scanf("%s", id);

        // Ouverture des fichiers nécessaires
        pt = fopen(filepatient, "r");
        cache = fopen("cache.txt", "a+");
        temp = fopen("temp.txt", "w");

        if (pt == NULL || cache == NULL || temp == NULL) {
            gotoxy(30, 10);
            textcolor(4);
            printf("Erreur lors de l'ouverture des fichiers !");
            if (pt) fclose(pt);
            if (cache) fclose(cache);
            if (temp) fclose(temp);
            Sleep(2000);
            continue;  // Retry instead of recursive call
        }

        // Lecture et traitement des données
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pt) != NULL) {
            int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%19s\t%19s\t%d/%d/%d\t%d:%d\n",
                          p.patient.nom, p.patient.prenom, p.patient.cin, p.patient.tel,
                          &p.jour, &p.mois, &p.annee, &p.heure, &p.minute);

            if (res == 9) {
                if (strcmp(id, p.patient.cin) == 0) {
                    trouve = 1;
                    fprintf(cache, "%s\t%s\t%s\t%s\t%02d/%02d/%02d\t%02d:%02d\n",
                            p.patient.nom, p.patient.prenom, p.patient.cin,
                            p.patient.tel, p.jour, p.mois, p.annee, p.heure, p.minute);
                } else {
                    fprintf(temp, "%s\t%s\t%s\t%s\t%02d/%02d/%02d\t%02d:%02d\n",
                            p.patient.nom, p.patient.prenom, p.patient.cin,
                            p.patient.tel, p.jour, p.mois, p.annee, p.heure, p.minute);
                }
            }
        }

        // Fermeture des fichiers
        fclose(cache);
        fclose(pt);
        fclose(temp);

        if (trouve) {
            gotoxy(30, 11);
            textcolor(12);
            printf("Suppression reussie !");
            Sleep(2000);
            if (remove(filepatient) == 0 && rename("temp.txt", filepatient) == 0) {
                afficherMenuSecretaire();
                return;
            } else {
                printf("Erreur lors de la mise à jour du fichier!");
                Sleep(2000);
            }
        } else {
            gotoxy(30, 11);
            textcolor(4);
            printf("Patient non existant, veuillez reessayer...");
            Sleep(3000);
        }
    } while (!trouve);
}


void addfacture()
{
    facture f;
    FILE *ff = fopen("factures.txt","a+");
    fflush(stdin);
    afficherTitre("Ajouter une facture");
    gotoxy(30,8);
    textcolor(11);
    printf("Donner cin du patient : ");
    gotoxy(30,9);
    textcolor(7);
    fgets(f.cin, sizeof(f.cin), stdin);
    f.cin[strcspn(f.cin, "\n")] = 0;
    fflush(stdin);
    gotoxy(30,10);
    textcolor(11);
    printf("donner la date du rendez vous(jour/mois/annee) :  ");
    gotoxy(30,11);
    textcolor(7);
    scanf("%d/%d/%d",&f.d.j,&f.d.m,&f.d.a);
    gotoxy(30,12);
    textcolor(11);
    printf("donner le prix : ");
    gotoxy(30,13);
    textcolor(7);
    scanf("%lf",&f.montant);
    fprintf(ff,"%s|%d/%d/%d|%lf\n",f.cin,f.d.j,f.d.m,f.d.a,f.montant);
    fflush(ff);
    system("cls");
    afficherTitre("Ajoute avec success !");
    Sleep(3000);
    afficherMenuSecretaire();
}

// Fonction pour changer la couleur du texte
void textcolor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

// Fonction pour positionner le curseur
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fonction pour afficher le titre du programme
void afficherTitre(const char *titre) {
    system("cls"); // Efface l'écran
    gotoxy(30, 2); // Placer le titre
    textcolor(9); // Bleu pour le titre
    printf("===================================");
    gotoxy(30, 3);
    printf("  %s", titre);
    gotoxy(30, 4);
    printf("===================================");
    textcolor(7); // Retour à la couleur par défaut
}

// Fonction pour afficher les options de menu
void afficherMenu(const char **options, int taille, int choixActuel) {
    for (int i = 0; i < taille; i++) {
        gotoxy(35, 6 + i); // Positionner chaque choix verticalement
        if (i == choixActuel) {
            textcolor(11); // Bleu clair pour le choix actuel
            printf("-> %s", options[i]);
            textcolor(7); // Couleur par défaut pour le reste
        } else {
            printf("   %s", options[i]);
        }
    }
}

// Fonction pour afficher la barre de chargement avec cadre
void afficherLoadingBar() {
    const int barLength = 50; // Longueur de la barre de chargement
    char loading[] = "[                                                  ]";
    int i;

    system("cls");
    // Afficher le cadre autour du message de chargement
    gotoxy(25, 8);
    textcolor(9); // Bleu pour le cadre
    printf("+--------------------------------------------------+");
    gotoxy(25, 9);
    printf("|                                                  |");
    gotoxy(25, 10);
    printf("|    Chargement en cours... 0%%                    |");
    gotoxy(25, 11);
    printf("|                                                  |");
    gotoxy(25, 12);
    printf("+--------------------------------------------------+");

    // Animation de la barre de chargement
    for (i = 0; i <= barLength; i++) {
        loading[i + 1] = '=';  // Ajoute des '=' dans la barre de chargement
        system("cls");

        // Redessiner le cadre
        gotoxy(25, 8);
        textcolor(9); // Bleu pour le cadre
        printf("+--------------------------------------------------+");
        gotoxy(25, 9);
        printf("|                                                  |");

        // Mettre à jour le pourcentage de chargement

        gotoxy(25, 10);
        printf("|    Chargement en cours... %d%%                   |", (i * 100) / barLength);
        gotoxy(25, 11);
        printf("|                                                  |");
        gotoxy(25, 12);
        printf("+--------------------------------------------------+");

        // Afficher la barre de chargement
        gotoxy(27, 10);
        printf("%s", loading);

        Sleep(50); // Attendre 50ms pour créer l'effet de chargement
    }
    system("cls");
    textcolor(7); // Retour à la couleur par défaut
}

// Fonction pour afficher le message central avec cadre
void afficherMessage() {
    system("cls");
    gotoxy(20, 8); // Placer le message au centre de l'écran
    textcolor(12); // Bleu foncé pour le message
    printf("+----------------------------------------------------+");
    gotoxy(20, 9);
    printf("| DigitalCabinet est une application developpee      |");
    gotoxy(20, 10);
    printf("| entierement par C, elle permet de gerer un         |");
    gotoxy(20, 11);
    printf("| cabinet medical ce qui facilite les taches pour    |");
    gotoxy(20, 12);
    printf("| le docteur, la secretaire, et plus essentiellement |");
    gotoxy(20, 13);
    printf("| le patient.                                        |");
    gotoxy(20, 14);
    printf("+----------------------------------------------------+");
    Sleep(2000); // Pause de 2 secondes pour afficher le message
    textcolor(7); // Retour à la couleur par défaut
}

// Fonction pour afficher le message des développeurs dans un carré
void afficherDeveloppeurs() {
    system("cls");
    // Cadrer le message des développeurs dans un carré
    gotoxy(30, 8);
    textcolor(14); // Jaune pour les noms des développeurs
    printf("+--------------------------------------------------+");
    gotoxy(30, 9);
    printf("| Developpe par : Marouane Ben Haddou              |");
    gotoxy(30, 10);
    printf("|               Nassima Maarouf                    |");
    gotoxy(30, 11);
    printf("|               Amine Elgardoum                    |");
    gotoxy(30, 12);
    printf("+--------------------------------------------------+");
    Sleep(2000); // Pause de 2 secondes pour afficher les développeurs
    textcolor(7); // Retour à la couleur par défaut
}

// Fonction pour afficher le menu du patient
void afficherPatientMenu() {
    const char *options[] = {"Prendre un rendez-vous", "Quitter vers menu principal"};
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Menu Patient"); // Display "Patient" menu title
        afficherMenu(options, tailleOptions, choixActuel); // Show menu options

        touche = getch(); // Read user input

        if (touche == 224) { // Handle arrow keys
            touche = getch();
            switch (touche) {
                case 72: // Up arrow
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Down arrow
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Enter key
            system("cls");
            if (choixActuel == 0) {
                addRendezvous();
                // Call the addRendezvous() function here
            } else if (choixActuel == 1) {
                return; // Go back to the main menu
            }
        }
    } while (1); // Loop to keep the menu active
}

// Fonction pour afficher le menu du docteur
void addfichiermed() {
    DossierMedical dossier;
    system("cls");
    afficherTitre("Ajout d'une fiche medicale");

    // Demander la CIN du patient
    gotoxy(30, 9);
    printf("Entrer la CIN du patient : ");
    textcolor(10); // Vert pour la saisie
    gotoxy(60, 9);
    scanf("%s", dossier.patient_id);
    textcolor(7); // Remettre en blanc

    // Construire le nom du fichier basé sur la CIN
    char nomFichier[30];
    snprintf(nomFichier, sizeof(nomFichier), "%s.txt", dossier.patient_id);

    // Ouvrir le fichier en lecture et écriture
    FILE *file = fopen(nomFichier, "r+");
    if (file == NULL) {
        // Si le fichier n'existe pas, créer un nouveau fichier
        file = fopen(nomFichier, "w");
        if (file == NULL) {
            gotoxy(30, 11);
            textcolor(12); // Rouge pour erreur
            perror("Erreur lors de l'ouverture ou de la création du fichier médical");
            return;
        }
        dossier.nb_consultations = 1; // Première consultation
    } else {
        // Lire le nombre de consultations existantes
        char buffer[256];
        dossier.nb_consultations = 0;

        while (fgets(buffer, sizeof(buffer), file)) {
            if (strstr(buffer, "Consultation #")) {
                dossier.nb_consultations++;
            }
        }
        dossier.nb_consultations++; // Incrémenter pour la nouvelle consultation
        fclose(file);
        file = fopen(nomFichier, "a"); // Réouvrir en mode ajout
    }

    // Récupérer la date actuelle
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(dossier.consultations[0].date_consultation, sizeof(dossier.consultations[0].date_consultation),
             "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    // Saisie des détails de la consultation
    gotoxy(30, 13);
    textcolor(11);
    printf("Consultation #%d\n", dossier.nb_consultations);

    gotoxy(30, 15);
    textcolor(11);
    printf("Motif : ");
    textcolor(10); // Vert pour la saisie
    gotoxy(50, 15);
    getchar(); // Vider le buffer
    fgets(dossier.consultations[0].motif, sizeof(dossier.consultations[0].motif), stdin);
    strtok(dossier.consultations[0].motif, "\n");
    textcolor(7); // Remettre en blanc

    gotoxy(30, 17);
    textcolor(11);
    printf("Diagnostic : ");
    textcolor(10);
    gotoxy(50, 17);
    fgets(dossier.consultations[0].diagnostic, sizeof(dossier.consultations[0].diagnostic), stdin);
    strtok(dossier.consultations[0].diagnostic, "\n");
    textcolor(7);

    gotoxy(30, 19);
    textcolor(11);
    printf("Traitement : ");
    textcolor(10);
    gotoxy(50, 19);
    fgets(dossier.consultations[0].traitement, sizeof(dossier.consultations[0].traitement), stdin);
    strtok(dossier.consultations[0].traitement, "\n");
    textcolor(7);

    gotoxy(30, 21);
    textcolor(11);
    printf("Notes : ");
    textcolor(10);
    gotoxy(50, 21);
    fgets(dossier.consultations[0].notes, sizeof(dossier.consultations[0].notes), stdin);
    strtok(dossier.consultations[0].notes, "\n");
    textcolor(7);

    // Ajouter la nouvelle consultation dans le fichier
    fprintf(file, "\nConsultation #%d\n", dossier.nb_consultations);
    fprintf(file, "Date : %s\n", dossier.consultations[0].date_consultation);
    fprintf(file, "Motif : %s\n", dossier.consultations[0].motif);
    fprintf(file, "Diagnostic : %s\n", dossier.consultations[0].diagnostic);
    fprintf(file, "Traitement : %s\n", dossier.consultations[0].traitement);
    fprintf(file, "Notes : %s\n", dossier.consultations[0].notes);
    fprintf(file, "-------------------------\n");

    // Fermeture du fichier
    fclose(file);

    gotoxy(30, 23);
    textcolor(14);
    printf("Dossier medical enregistre avec success dans %s\n", nomFichier);

    Sleep(3000);
    DocMenu();
}

void afficherDocteurMenu() {
    const char *options[] = {"Se connecter", "S\'inscrire", "Quitter vers menu principal"};
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Menu Docteur"); // Display "Docteur" menu title
        afficherMenu(options, tailleOptions, choixActuel); // Show menu options

        touche = getch(); // Read user input

        if (touche == 224) { // Handle arrow keys
            touche = getch();
            switch (touche) {
                case 72: // Up arrow
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Down arrow
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Enter key
            system("cls");
            if (choixActuel == 0) {
                Connecter_Doc();
                // Implement login logic here
            } else if (choixActuel == 1) {
                adddoc();
                // Implement registration logic here
            } else if (choixActuel == 2) {
                return; // Go back to the main menu
            }
        }
    } while (1); // Loop to keep the menu active
}
void DocMenu() {
    const char *options[] = {
        "Voir la liste des patients",
        "Ajouter une fiche medicale",
        "Retour au menu principal"
    };
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Menu Docteur - Connexion Reussie"); // Titre après connexion
        afficherMenu(options, tailleOptions, choixActuel); // Afficher les options du menu

        touche = getch(); // Lire l'entrée de l'utilisateur

        if (touche == 224) { // Gérer les touches fléchées
            touche = getch();
            switch (touche) {
                case 72: // Flèche haut
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Flèche bas
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Touche Entrée
            system("cls");
            switch (choixActuel) {
                case 0:
                    rdvtoday();
                    // Appeler la fonction pour afficher la liste des patients ici
                    break;
                case 1:
                    addfichiermed();
                    // Appeler la fonction pour ajouter une fiche médicale ici
                    break;
                case 2:
                    return; // Retour au menu principal
            }
        }
    } while (1); // Boucle pour garder le menu actif
}

// Fonction pour afficher le menu du secrétaire
void afficherSecretaireMenu() {
    const char *options[] = {"S'authentifier", "Retour au menu principal"};
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Menu Secretaire"); // Display "Secrétaire" menu title
        afficherMenu(options, tailleOptions, choixActuel); // Show menu options

        touche = getch(); // Read user input

        if (touche == 224) { // Handle arrow keys
            touche = getch();
            switch (touche) {
                case 72: // Up arrow
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Down arrow
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Enter key
            system("cls");
            if (choixActuel == 0) {
                afficherAuthentificationSecretaire(); // Call authentication function
            } else if (choixActuel == 1) {
                return; // Go back to the main menu
            }
        }
    } while (1); // Loop to keep the menu active
}

// Fonction pour l'authentification du secrétaire
void afficherAuthentificationSecretaire() {
    char cin[20];
    system("cls");
    afficherTitre("Authentification Secretaire");
    gotoxy(30, 8);
    textcolor(11); // Bleu pour le texte
    printf("Veuillez entrer le CIN de votre patron : ");
    textcolor(7); // Retour à la couleur par défaut
    gotoxy(30, 9);
    scanf("%s", cin); // Demander le CIN
    if (authentifier(cin)){// Ici vous pouvez ajouter une validation du CIN si nécessaire
    afficherMenuSecretaire(); // Aller au menu du secrétaire après l'authentification
    }
    else
    {
        gotoxy(30,10);
        textcolor(4);
        printf("authentification echoue , ressayer !");
        Sleep(4000);
        afficherAuthentificationSecretaire();
    }
}

void rdvtoday() {
    system("cls");
    int j, m, a;
    int exist = 0;
    char buffer[256]; // Buffer pour lire une ligne complète du fichier
    int x_start = 20; // Position de départ pour centrer le tableau
    int y = 5;        // Position de départ en hauteur

    // Ouvrir le fichier
    FILE *pt = fopen(filepatient, "r");
    if (pt == NULL) {
        textcolor(4); // Rouge pour les messages d'erreur
        gotoxy(x_start, y);
        printf("Erreur : Impossible d'ouvrir le fichier des patients.\n");
        getchar();
        return;
    }

    // Saisie de la date
    gotoxy(x_start, y++);
    textcolor(9); // Bleu clair
    printf("Votre saisie (jour/mois/annee) : ");
    if (scanf("%d/%d/%d", &j, &m, &a) != 3) {
        gotoxy(x_start, y++);
        textcolor(4);
        printf("Erreur : Format de date invalide.\n");
        fclose(pt);
        return;
    }
    system("cls");
    // Affichage des en-têtes du tableau
    textcolor(1); // Bleu foncé
    gotoxy(x_start, y++);
    printf("--------------- Rendez-vous du %02d/%02d/%04d ---------------", j, m, a);
    gotoxy(x_start, y++);
    printf("----------------------------------------------------------");
    gotoxy(x_start, y++);
    printf("| %-20s | %-15s | %-10s |", "Nom et Prenom", "CIN", "Heure");
    gotoxy(x_start, y++);
    printf("----------------------------------------------------------");

    // Lecture des lignes du fichier
    while (fgets(buffer, sizeof(buffer), pt) != NULL) {
        // Extraire les données de la ligne
        int res = sscanf(buffer, "%[^\t]\t%[^\t]\t%19s\t%19s\t%d/%d/%d\t%d:%d",
                         p.patient.nom, p.patient.prenom, p.patient.cin, p.patient.tel,
                         &p.jour, &p.mois, &p.annee, &p.heure, &p.minute);

        // Vérifier si les données sont correctes
        if (res == 9 && p.jour == j && p.mois == m && p.annee == a) {
            exist = 1;
            textcolor(3); // Cyan pour les données trouvées
            gotoxy(x_start, y++);
            printf("| %-20s | %-15s | %02d:%02d      |",
                   strcat(strcat(p.patient.nom, " "), p.patient.prenom), p.patient.cin, p.heure, p.minute);
        }
    }

    // Message si aucun rendez-vous trouvé
    if (!exist) {
        textcolor(4); // Rouge
        gotoxy(x_start, y++);
        printf("| %-47s |", "Pas de rendez-vous pour cette date !");
    }

    textcolor(1); // Bleu foncé pour la fin du tableau
    gotoxy(x_start, y++);
    printf("----------------------------------------------------------");

    fclose(pt);
    textcolor(7); // Couleur par défaut
    gotoxy(x_start, y++);
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar(); // Consommer le retour à la ligne de scanf
    getchar(); // Attendre une nouvelle entrée
    system("cls");
    menu();
}




// Fonction pour afficher le menu des actions du secrétaire
void afficherMenuSecretaire() {
    const char *options[] = {
        "Ajouter un rendez-vous",
        "Supprimer un rendez-vous",
        "Afficher rendez-vous d'une date",
        "Modifier les infos d'un patient",
        "Ajouter une facture",
        "Charger une facture",
        "Quitter"
    };
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Menu Secretaire - Actions"); // Afficher le titre du menu des actions
        afficherMenu(options, tailleOptions, choixActuel); // Afficher les options du menu

        touche = getch(); // Lire l'entrée de l'utilisateur

        if (touche == 224) { // Gérer les touches fléchées
            touche = getch();
            switch (touche) {
                case 72: // Flèche haut
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Flèche bas
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Touche Entrée
            system("cls");
            switch (choixActuel) {
                case 0:
                    addRendezvous();
                    break;
                case 1:
                    supprimerrdv();
                    break;
                case 2:
                    rdvtoday();
                    break;
                case 3:
                    modifierpatient();
                    break;
                case 4:
                   addfacture();
                    break;
                case 5:
                    afficherfacture();
                    break;
                case 6:
                    return; // Quitter
            }
        }
    } while (1); // Boucle pour garder le menu actif
}

// Fonction principale
void menu() {

    const char *options[] = {
        "Patient",
        "Docteur",
        "Secretaire",
        "Quitter"
    };
    int tailleOptions = sizeof(options) / sizeof(options[0]);
    int choixActuel = 0;
    int touche;

    do {
        afficherTitre("Bienvenue dans DigitalCabinet");
        afficherMenu(options, tailleOptions, choixActuel);

        touche = getch(); // Lire l'entrée de l'utilisateur

        if (touche == 224) { // Gérer les touches fléchées
            touche = getch();
            switch (touche) {
                case 72: // Flèche haut
                    choixActuel--;
                    if (choixActuel < 0) choixActuel = tailleOptions - 1;
                    break;
                case 80: // Flèche bas
                    choixActuel++;
                    if (choixActuel >= tailleOptions) choixActuel = 0;
                    break;
            }
        } else if (touche == 13) { // Touche Entrée
            system("cls");
            switch (choixActuel) {
                case 0:
                    afficherPatientMenu();
                    break;
                case 1:
                    afficherDocteurMenu();
                    break;
                case 2:
                    afficherSecretaireMenu();
                    break;
                case 3:
                    exit(0); // Quitter
            }
        }
    } while (1); // Boucle principale
}
void afficherfacture() {
    system("cls");
    time_t t;
    struct tm *tm_info;
    char date[26];

    time(&t);
    tm_info = localtime(&t);
    strftime(date, 26, "%d/%m/%Y %H:%M:%S", tm_info);
    int ex = 0;  // Indicateur si une facture existe pour le CIN donné
    Personne pr;

    // Saisie des informations de l'utilisateur
    afficherTitre("Saisie des donnees");
    gotoxy(30, 8); textcolor(11); printf("Donner nom :");
    gotoxy(30, 9); textcolor(7);
    fflush(stdin);
    fgets(pr.nom,sizeof(pr.nom),stdin);
    pr.nom[strcspn(pr.nom, "\n")] = 0;
    fflush(stdin);
    gotoxy(30, 10); textcolor(11); printf("Donner Prenom :");
    gotoxy(30, 11); textcolor(7); scanf("%s", pr.prenom);
    gotoxy(30, 12); textcolor(11); printf("Donner CIN :");
    gotoxy(30, 13); textcolor(7); scanf("%s", pr.cin);
    gotoxy(30, 14); textcolor(11); printf("Donner numero de telephone :");
    gotoxy(30, 15); textcolor(7); scanf("%s", pr.tel);

    double total = 0.00;  // Total des montants
    facture f;
    char buffer[256];  // Buffer pour lire une ligne complète
    FILE *fac = fopen("factures.txt", "r");

    // Vérification de l'ouverture du fichier
    if (fac == NULL) {
        gotoxy(30, 16);
        printf("Aucune facture existante");
        Sleep(3000);
        afficherMenuSecretaire();
        return;  // Quitter la fonction
    }

    // Calcul des largeurs dynamiques
    int max_nom = strlen("Nom");
    int max_prenom = strlen("Prenom");
    int max_cin = strlen("CIN");
    int max_tel = strlen("Tel");
    int max_montant = strlen("Montant total à payer");

    // Mise à jour des largeurs si nécessaire
    if ((int)strlen(pr.nom) > max_nom) max_nom = strlen(pr.nom);
    if ((int)strlen(pr.prenom) > max_prenom) max_prenom = strlen(pr.prenom);
    if ((int)strlen(pr.cin) > max_cin) max_cin = strlen(pr.cin);
    if ((int)strlen(pr.tel) > max_tel) max_tel = strlen(pr.tel);

    // Lecture ligne par ligne
    while (fgets(buffer, sizeof(buffer), fac) != NULL) {
        // Suppression du saut de ligne
        buffer[strcspn(buffer, "\n")] = 0;

        // Extraction des données avec sscanf
        if (sscanf(buffer, "%[^|]|%d/%d/%d|%lf", f.cin, &f.d.j, &f.d.m, &f.d.a, &f.montant) == 5) {
            if (strcmp(pr.cin, f.cin) == 0) {  // Comparaison des CIN
                ex = 1;  // Facture trouvée
                total += f.montant;

                // Mise à jour des largeurs en fonction des données
                if ((int)strlen(f.cin) > max_cin) max_cin = strlen(f.cin);
            }
        }
    }

    fclose(fac);  // Fermeture du fichier

    // Calcul de la largeur totale du tableau
    int width = max_nom + max_prenom + max_cin + max_tel + max_montant + 15;

    // Affichage des résultats
    if (ex) {
        system("cls");
        textcolor(9);
        afficherTitre("Facture Totale");

        // Affichage dynamique avec bordures ajustées
        gotoxy(30, 8);  printf("+%.*s+", width, "----------------------------------------------------------");
        gotoxy(30, 9);  printf("| %-*s | %-*s | %-*s |", max_nom, "Nom", max_prenom, "Prenom", max_cin, "CIN");
        gotoxy(30, 10); printf("+%.*s+", width, "----------------------------------------------------------");
        gotoxy(30, 11); printf("| %-*s | %-*s | %-*s |", max_nom, pr.nom, max_prenom, pr.prenom, max_cin, pr.cin);
        gotoxy(30, 12); printf("+%.*s+", width, "----------------------------------------------------------");
        gotoxy(30, 13); printf("| Montant total a payer : %-*.2f DH |", max_montant, total);
        gotoxy(30, 14); printf("+%.*s+", width, "----------------------------------------------------------");
        char filename[50];
        snprintf(filename, sizeof(filename), "%sfact.pdf", pr.cin);  // Nom du fichier basé sur le CIN
        FILE *pdf = fopen(filename, "w");
     fprintf(pdf, "%%PDF-1.4\n");
    fprintf(pdf, "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");
    fprintf(pdf, "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");
    fprintf(pdf, "3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 4 0 R >>\nendobj\n");
    fprintf(pdf, "4 0 obj\n<< /Length 5 0 R >>\nstream\n");
    fprintf(pdf, "BT /F1 24 Tf 100 700 Td (Facture Totale) Tj ET\n");

    // Ajout de la date
    fprintf(pdf, "BT /F1 12 Tf 100 670 Td (Date d'impression: %s) Tj ET\n", date);

    // Information du patient
    fprintf(pdf, "BT /F1 12 Tf 100 650 Td (Nom: %s) Tj ET\n", pr.nom);
    fprintf(pdf, "BT /F1 12 Tf 100 630 Td (Prenom: %s) Tj ET\n", pr.prenom);
    fprintf(pdf, "BT /F1 12 Tf 100 610 Td (CIN: %s) Tj ET\n", pr.cin);
    fprintf(pdf, "BT /F1 12 Tf 100 590 Td (Telephone: %s) Tj ET\n", pr.tel);
    fprintf(pdf, "BT /F1 12 Tf 100 570 Td (Montant total a payer: %.2f DH) Tj ET\n", total);

    // Ajout du message de remerciement en bas de page
    fprintf(pdf, "BT /F1 14 Tf 100 200 Td (Nous vous remercions de votre confiance !) Tj ET\n");
    fprintf(pdf, "BT /F1 12 Tf 100 180 Td (Pour toute question, n'hesitez pas a nous contacter.) Tj ET\n");

    fprintf(pdf, "endstream\nendobj\n");
    fprintf(pdf, "5 0 obj\n12\nendobj\n");
    fprintf(pdf, "xref\n0 6\n0000000000 65535 f \n0000000010 00000 n \n0000000067 00000 n \n");
    fprintf(pdf, "0000000117 00000 n \n0000000173 00000 n \n0000000250 00000 n \n");
    fprintf(pdf, "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n288\n%%EOF\n");
    fclose(pdf);
    } else {
        system("cls");
        gotoxy(30, 10);
        textcolor(4);
        printf("Aucune facture existante. Réessayez de nouveau.");
        Sleep(5000);
        afficherMenuSecretaire();
    }

    gotoxy(30, 17);
    textcolor(10);  // Texte vert pour remerciement
    printf("Merci pour votre visite !");
    textcolor(15);  // Réinitialisation des couleurs par défaut
    getch();
    afficherMenuSecretaire();
}

int main()
{
     afficherLoadingBar();
    afficherMessage();
    afficherDeveloppeurs();
    menu();
    return 0;
}
