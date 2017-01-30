/*
 * Editor language FR library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Interface.h"


FORK_EXPORT const wchar_t* LanguageName()
{
    return L"Fran�ais";
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
        L"G�n�re un nouveau projet.",

        L"&Ouvrir ...\tCtrl+O",
        L"Ouvre un projet existant � partir du fichier.",

        L"&Enregistrer\tCtrl+S",
        L"Enregistre le projet actuel.",

        L"Enregistrer &Sous ...",
        L"Enregistre le projet actuel sous un nouveau nom de fichier.",

        L"&Param�tres du Projet ...",
        L"Ouvre la fen�tre de param�tres du projet.",

        L"&Quitter",
        L"Quitte l'application enti�re.",

        /* --- Main menu: edit --- */
        L"&Modifier",

        L"&Annuler\tCtrl+Z",
        L"Annuler t�che pr�c�dente.",
        L"&R�tablir.\tCtrl+Y",
        L"Refaire t�che pr�c�dente.",

        L"&Couper\tCtrl+X",
        L"Coupe tous les objets s�lectionn�s dans le presse-papiers.",
        L"C&opie\tCtrl+C",
        L"Copiez tous les objets s�lectionn�s dans le presse-papiers.",
        L"Co&ller\tCtrl+V",
        L"Coller tous les objets du presse-papiers.",

        L"&Effacer\tDel",
        L"Supprimer tous les objets s�lectionn�s.",

        /* --- Main menu: help --- */
        L"&Assistance",

        L"&Manuel",
        L"Affiche le manuel auxiliaire.",

        L"L'Aide en &Ligne.",
        L"Ouvre l'aide en ligne dans le navigateur Web.",

        L"&Afficher l'Astuce",
        L"Affiche la bo�te de dialogue de pointe.",

        L"&Informations",
        L"Affiche la bo�te de dialogue d'information.",

        /* --- About dialog --- */
        L"ForkEd est le (WYSIWYG) �diteur de ForkENGINE",

        /* --- Basics --- */
        L"Notions de Base",
        L"Nom",
        L"Activ�",

        /* --- Transformation --- */
        L"Transformation",
        L"Position",
        L"Rotation",
        L"�caillage",
    };

    if (index >= sizeof(stringList)/sizeof(stringList[0]))
        return NULL;

    return stringList[index];
}



// ========================