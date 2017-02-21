/*
  Copyright (C) 2017 SUSE LLC

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/

#include "YHttpAppHandler.h"
#include "YJsonSerializer.h"
#include "YApplication.h"

#define YUILogComponent "http-ui"
#include "YUILog.h"

#include <microhttpd.h>
#include <json/json.h>

void YHttpAppHandler::body(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size, std::ostream& body)
{
    Json::Value info;
    YApplication *app = YUI::app();

    info["animation_support"] = app->hasAnimationSupport();
    info["application_icon"] = app->applicationIcon();
    info["application_title"] = app->applicationTitle();
    info["debug_log"] = YUILog::debugLoggingEnabled();
    info["default_height"] = app->defaultHeight();
    info["default_width"] = app->defaultWidth();
    info["display_colors"] = app->displayColors();
    info["display_depth"] = app->displayDepth();
    info["display_height"] = app->displayHeight();
    info["display_width"] = app->displayWidth();
    info["icon_path"] = app->iconBasePath();
    info["icon_support"] = app->hasIconSupport();
    info["image_support"] = app->hasImageSupport();
    info["language"] = app->language();
    info["left_handed_mouse"] = app->leftHandedMouse();
    info["product_name"] = app->productName();
    info["rich_text_table_support"] = app->richTextSupportsTable();
    info["text_mode"] = app->isTextMode();
    info["utf8_support"] = app->hasFullUtf8Support();
    info["wizard_support"] = app->hasWizardDialogSupport();

    std::map<std::string,std::string> relnotes = app->releaseNotes();
    if (!relnotes.empty()) {
        Json::Value relnotes_json;
        
        for(const auto &pair: relnotes) {
            info[pair.first] = pair.second;
        }
        
        info["release_notes"] = relnotes_json;
    }
    
    YJsonSerializer::save(info, body);
}

std::string YHttpAppHandler::contentEncoding()
{
    return "application/json";
}

int YHttpAppHandler::errorCode()
{
    return MHD_HTTP_OK;
}
