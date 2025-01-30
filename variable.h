#ifndef VARIABLE_H
#define VARIABLE_H
#include<stdio.h>
#include<time.h>

const char *filepatient = "patient.txt";
 FILE *pt;  // Patient file pointer (initialized elsewhere)
 FILE *sec;  // Secretary file pointer (initialized elsewhere)
 FILE *doc;  // Doctor file pointer (initialized elsewhere)

// Structure Definitions
typedef struct {
    char nom[20];
    char prenom[20];
    char cin[20];
    char tel[20];  // Field for phone number
} Personne;

typedef struct {
    int jour;
    int mois;
    int annee;
    int heure;
    int minute;
    Personne patient;  // Patient details
} rdv;
rdv p;
typedef struct {
    char id[20];
    char prenom[20];
    char nom[20];
} Docteur;

typedef struct {
    int j;
    int m;
    int a;
} date;
typedef struct {
    char date_consultation[11]; // Date de la consultation
    char motif[100]; // Raison de la consultation
    char diagnostic[200]; // Diagnostic posé
    char traitement[200]; // Traitement prescrit
    char notes[300]; // Notes supplémentaires
} Consultation;

typedef struct {
    char patient_id[20]; // Identifiant du patient
    int nb_consultations; // Nombre de consultations enregistrées
    Consultation consultations[100]; // Tableau fixe ou pointer vers un tableau dynamique
} DossierMedical;
typedef struct {
    char cin[20];
    char nom[20];
    char prenom[50];
    date d;
    double montant;
} facture;

// Function Prototypes
// Check if patient exists
int estBissextile(int annee);  // Check if year is leap year
int estDimanche(int *jour, int *mois, int *annee);  // Check if day is Sunday
int estHeureValide(int *heure, int *minute);  // Validate time
void addRendezvous();  // Add appointment
int nmbrdoccurence(const char *filepatient);  // Count occurrences
int authentifier(char id[]);  // Authenticate user
void firstmenu();  // Display first menu
void afficherMenu();  // Display menu
void supprimerPatient(char idsupprimer[]);  // Delete patient
int occrdv(int j , int m , int a);  // Appointment occurrence
int siplein(int j , int m ,int a );
void rdvtoday();  // Display today's appointments
void supprimerrdv();  // Delete appointment
void afficherfacture();  // Display invoice
void addfacture();  // Add invoice
void Connecter_Doc();  // Connect doctor
int Doc_Existe( char id[]);  // Check if doctor exists
void Afficher_Patient();  // Display patient
 // Doctor menu
void addfacture();
void obtenirDateActuelle(int *jour, int *mois, int *annee);
void affichefacture();
int occrdv (int j, int m , int a);
void supprimerrdv();
void addfichiermed();  // Coming soon
void adddoc();
void gpat();
void gsec();
void gdoc();
void Connecter_Doc() ;
int Doc_Existe(char id[]);
void menu();
void affichefacture();
void afficherMenuSecretaire();
void afficherAuthentificationSecretaire();
void afficherMenuSecretaire() ;
void afficherDocteurMenu();
int existerdv(int jour,int mois,int annee,int heure,int minute);
void DocMenu();
void textcolor(int color);
void gotoxy(int x, int y);
void adddoc();
void Connecter_Doc();
void afficherTitre(const char *titre);
void affichermenu(int choixActuel);
void modifierpatient();
#endif
