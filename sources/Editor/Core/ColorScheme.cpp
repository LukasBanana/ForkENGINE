/*
 * Editor color scheme file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ColorScheme.h"
#include "Lang/XMLParser/XMLParser.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"
#include "Video/Core/BitwiseColor.h"

#include "UIExtensions/UIPropertyGrid.h"
#include "UIExtensions/UIViewManager.h"

#include <map>
#include <initializer_list>


namespace Fork
{

namespace Editor
{


using namespace Lang::AbstractSyntaxTrees;

std::vector<ColorScheme> LoadColorSchemes(const std::string& filename)
{
    static const std::string colorSchemeIdent = "color_scheme";

    /* Establish color hash-map */
    std::map<std::string, size_t> colorOffsets;

    auto AddOffsetList = [&](const std::initializer_list<const std::string>& names)
    {
        size_t index = 0;
        for (const auto& name : names)
            colorOffsets[name] = index++;
    };

    AddOffsetList({
        "lineColor",
        "captionColor",
        "gradientColor",
        "sashColor",
        "cellBgColor",
        "captionBgColor",
        "disabledTextColor",
        "selectionBgColor",
        "selectionTextColor",
        "captionTextColor",
        "cellTextColor"
    });

    /* Parse color scheme XML file */
    Lang::XMLParser xmlParser;
    auto xmlTags = xmlParser.ParseXMLFile(filename);

    std::vector<ColorScheme> colorSchemes;

    /* Parse all XML tags */
    for (const auto& tag : xmlTags)
    {
        if (tag->ident == colorSchemeIdent)
        {
            ColorScheme scheme;

            /* Parse attributes */
            for (const auto& attrib : tag->attributes)
            {
                if (attrib->ident == "name")
                    scheme.name = attrib->value;
                else
                    IO::Log::Warning("Unknown XML attribute \"" + attrib->ident + "\" in tag \"" + tag->ident + "\"");
            }

            /* Parse sub tags */
            for (const auto& node : tag->nodes)
            {
                auto subTag = dynamic_cast<XMLTag*>(node.get());
                if (subTag)
                {
                    if (subTag->ident == "color")
                    {
                        /* Parse color attributes */
                        std::string colorName;
                        Video::ColorRGBub colorValue;

                        for (const auto& attrib : subTag->attributes)
                        {
                            auto ValueToColor = [](const std::string& value)
                            {
                                return static_cast<unsigned char>(StrToNum<int>(value));
                            };

                            if (attrib->ident == "name")
                                colorName = attrib->value;
                            else if (attrib->ident == "gray")
                            {
                                const auto val = ValueToColor(attrib->value);
                                colorValue = { val, val, val };
                            }
                            else if (attrib->ident == "red")
                                colorValue.r = ValueToColor(attrib->value);
                            else if (attrib->ident == "green")
                                colorValue.g = ValueToColor(attrib->value);
                            else if (attrib->ident == "blue")
                                colorValue.b = ValueToColor(attrib->value);
                            else
                            {
                                IO::Log::Warning(
                                    "Unknown XML attribute \"" + attrib->ident + "\" in tag \"" +
                                    subTag->ident + "\" " + attrib->Pos().GetString()
                                );
                            }
                        }

                        /* Store color in scheme */
                        if (!colorName.empty())
                        {
                            auto it = colorOffsets.find(colorName);
                            if (it != colorOffsets.end())
                            {
                                /* Store color component in scheme structure data */
                                (scheme.ColorPtr() + it->second)->SetRGBA(Video::ColorToUInt32ABGR(colorValue));
                            }
                            else
                            {
                                IO::Log::Warning(
                                    "Unknown color scheme component \"" + colorName + "\" in XML tag \"" +
                                    subTag->ident + "\" " + subTag->Pos().GetString()
                                );
                            }
                        }
                        else
                        {
                            IO::Log::Warning(
                                "Missing attribute \"name\" in XML tag \"" +
                                subTag->ident + "\" " + subTag->Pos().GetString()
                            );
                        }
                    }
                    else
                    {
                        IO::Log::Warning("Unknown XML tag \"" + tag->ident + "\" in \"" + colorSchemeIdent + "\"");
                    }
                }
                else
                    IO::Log::Warning("Only sub tags are allowed in color scheme XML file");
            }

            /* Add new scheme to the list */
            colorSchemes.push_back(scheme);
        }
        else
            IO::Log::Warning("Unknown global XML tag \"" + tag->ident + "\" " + tag->Pos().GetString());
    }

    return colorSchemes;
}

void ChangeColorScheme(const ColorScheme& scheme)
{
    UIPropertyGrid::ChangeColorSchemeForAll(scheme);
    UIViewManager::ChangeColorSchemeForAll(scheme);
}


} // /namespace Editor

} // /namespace Fork



// ========================