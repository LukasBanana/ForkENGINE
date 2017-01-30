/*
 * Editor language DE library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Interface.h"


FORK_EXPORT const wchar_t* LanguageName()
{
    return L"Deutsch";
}

FORK_EXPORT const wchar_t* LanguageString(unsigned int index)
{
    static const wchar_t* stringList[] =
    {
        /* --- Common --- */
        L"Unbenannt",
        L"Standard",
        L"Allgemeines",
        L"Grafik",

        /* --- Main frame --- */
        L"Eigenschaften",
        L"Hierarchie",

        /* --- Main menu: project --- */
        L"&Projekt",

        L"&Neu\tStrg+N",
        L"Legt ein neues Projekt an.",

        L"&�ffnen ...\tStrg+O",
        L"�ffnet ein existierendes Projekt von einer Datei.",

        L"&Speichern\tStrg+S",
        L"Speichert das aktuelle Projekt.",

        L"Speichern &Als ...",
        L"Speichert das aktuelle Projekt unter einem neuen Dateinamen.",

        L"&Projekt Einstellungen ...",
        L"�ffnet das Fenster f�r Projekt Einstellungen.",

        L"&Beenden",
        L"Beendet die komplette Anwendung.",

        /* --- Main menu: edit --- */
        L"&Bearbeiten",

        L"&R�ckg�ngig\tStrg+Z",
        L"Macht letzten Schritt r�ckg�ngig.",
        L"&Wiederherstellen\tStrg+Y",
        L"Stellt den letzten Schritt wieder her.",

        L"&Ausschneiden\tStrg+X",
        L"Verschiebt alle selektierten Objekte in die Zwischenablage.",
        L"&Koppieren\tStrg+C",
        L"Kopiert alle selektierten Objekte in die Zwischenablage.",
        L"&Einf�gen\tStrg+V",
        L"F�gt alle Objekte aus der Zwischenablage ein.",

        L"&L�schen\tEntf",
        L"L�scht alle selektierten Objekte.",

        /* --- Main menu: help --- */
        L"&Hilfe",

        L"&Anleitung",
        L"Zeigt die Hilfsanleitung.",

        L"Online &Hilfe",
        L"�ffnet die Online Hilfe im web browser.",

        L"&Tipp Anzeigen",
        L"Zeigt den Tipp Dialog.",

        L"&�ber",
        L"Zeigt den Informations Dialog.",

        /* --- About dialog --- */
        L"ForkEd ist der ForkENGINE (WYSIWYG) Welt Editor",

        /* --- Basics --- */
        L"Grundlagen",
        L"Name",
        L"Aktiv",

        /* --- Transformation --- */
        L"Transformation",
        L"Position",
        L"Rotation",
        L"Skalierung",
    };

    if (index >= sizeof(stringList)/sizeof(stringList[0]))
        return NULL;

    return stringList[index];
}



// ========================