/*
 * Editor language FR library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Interface.h"


FORK_EXPORT const wchar_t* LanguageName()
{
    return L"Français";
}

FORK_EXPORT const wchar_t* LanguageString(unsigned int index)
{
    static const wchar_t* stringList[] =
    {
        /* --- Common --- */
        L"Anonyme",

        /* --- Main menu: project --- */
        L"&Projet",

        L"&Nouveau\tCtrl+N",
        L"Génère un nouveau projet.",

        L"&Ouvrir ...\tCtrl+O",
        L"Ouvre un projet existant à partir du fichier.",

        L"&Enregistrer\tCtrl+S",
        L"Enregistre le projet actuel.",

        L"Enregistrer &Sous ...",
        L"Enregistre le projet actuel sous un nouveau nom de fichier.",

        L"&Paramètres du Projet ...",
        L"Ouvre la fenêtre de paramètres du projet.",

        L"&Quitter",
        L"Quitte l'application entière.",

        /* --- Main menu: edit --- */
        L"&Modifier",

        L"&Annuler\tCtrl+Z",
        L"Annuler tâche précédente.",
        L"&Rétablir.\tCtrl+Y",
        L"Refaire tâche précédente.",

        L"&Couper\tCtrl+X",
        L"Coupe tous les objets sélectionnés dans le presse-papiers.",
        L"C&opie\tCtrl+C",
        L"Copiez tous les objets sélectionnés dans le presse-papiers.",
        L"Co&ller\tCtrl+V",
        L"Coller tous les objets du presse-papiers.",

        L"&Effacer\tDel",
        L"Supprimer tous les objets sélectionnés.",

        /* --- Main menu: help --- */
        L"&Assistance",

        L"&Manuel",
        L"Affiche le manuel auxiliaire.",

        L"L'Aide en &Ligne.",
        L"Ouvre l'aide en ligne dans le navigateur Web.",

        L"&Afficher l'Astuce",
        L"Affiche la boîte de dialogue de pointe.",

        L"&Informations",
        L"Affiche la boîte de dialogue d'information.",

        /* --- About dialog --- */
        L"ForkEd est le (WYSIWYG) éditeur de ForkENGINE",

        /* --- Basics --- */
        L"Notions de Base",
        L"Nom",
        L"Activé",

        /* --- Transformation --- */
        L"Transformation",
        L"Position",
        L"Rotation",
        L"Écaillage",
    };

    if (index >= sizeof(stringList)/sizeof(stringList[0]))
        return NULL;

    return stringList[index];
}



// ========================