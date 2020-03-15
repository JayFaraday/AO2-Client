#include "text_file_functions.h"

#include <bitset>

QString AOApplication::read_design_ini(QString p_identifier, QString p_design_path)
{
  QSettings settings(p_design_path, QSettings::IniFormat);
  QVariant value = settings.value(p_identifier);
  if (value.type() == QVariant::StringList) {
    return value.toStringList().join(",");
  } else {
    return value.toString();
  }
}

pos_size_type AOApplication::get_element_dimensions(QString p_identifier, QString p_file)
{
  QString design_ini_path = get_theme_path(p_file);
  QString default_path = get_default_theme_path(p_file);
  QString f_result = read_design_ini(p_identifier, design_ini_path);

  pos_size_type return_value;

  return_value.x = 0;
  return_value.y = 0;
  return_value.width = -1;
  return_value.height = -1;

  if (f_result == "")
  {
    f_result = read_design_ini(p_identifier, default_path);

    if (f_result == "")
      return return_value;
  }

  QStringList sub_line_elements = f_result.split(",");

  if (sub_line_elements.size() < 4)
    return return_value;

  return_value.x = sub_line_elements.at(0).toInt();
  return_value.y = sub_line_elements.at(1).toInt();
  return_value.width = sub_line_elements.at(2).toInt();
  return_value.height = sub_line_elements.at(3).toInt();

  return return_value;
}

QColor AOApplication::get_color(QString p_identifier, QString p_file)
{
  QString design_ini_path = get_theme_path(p_file);
  QString default_path = get_default_theme_path(p_file);
  QString f_result = read_design_ini(p_identifier, design_ini_path);

  QColor return_color(0, 0, 0);

  if (f_result == "")
  {
    f_result = read_design_ini(p_identifier, default_path);

    if (f_result == "")
      return return_color;
  }

  QStringList color_list = f_result.split(",");

  if (color_list.size() < 3)
    return return_color;

  return_color.setRed(color_list.at(0).toInt());
  return_color.setGreen(color_list.at(1).toInt());
  return_color.setBlue(color_list.at(2).toInt());

  return return_color;
}

QColor AOApplication::get_chat_color(QString p_identifier, QString p_chat)
{
  QColor return_color(255, 255, 255);

  if (p_identifier == "_inline_grey")
  {
    return_color = QColor(187, 187, 187);
  }
  else
  {
    switch (p_identifier.toInt()) {
      case 1:
        return_color = QColor(0, 255, 0);
        break;
      case 2:
        return_color = QColor(255, 0, 0);
        break;
      case 3:
        return_color = QColor(255, 165, 0);
        break;
      case 4:
        return_color = QColor(45, 150, 255);
        break;
      case 5:
        return_color = QColor(255, 255, 0);
        break;
      case 7:
        return_color = QColor(255, 192, 203);
        break;
      case 8:
        return_color = QColor(0, 255, 255);
        break;
      case 0:
      case 6: // 6 is rainbow.
      default:
        return_color = QColor(255, 255, 255);
        break;
    }
  }

  p_identifier = p_identifier.prepend("c");
  QString design_ini_path = get_base_path() + "misc/" + p_chat + "/config.ini";
  QString default_path = get_base_path() + "misc/default/config.ini";
  QString f_result = read_design_ini(p_identifier, design_ini_path);

  if (f_result == "")
  {
    f_result = read_design_ini(p_identifier, default_path);

    if (f_result == "")
      return return_color;
  }

  QStringList color_list = f_result.split(",");

  if (color_list.size() < 3)
    return return_color;

  return_color.setRed(color_list.at(0).toInt());
  return_color.setGreen(color_list.at(1).toInt());
  return_color.setBlue(color_list.at(2).toInt());

  return return_color;
}

QString AOApplication::get_sfx(QString p_identifier)
{
  QString design_ini_path = get_theme_path("courtroom_sounds.ini");
  QString default_path = get_default_theme_path("courtroom_sounds.ini");
  QString f_result = read_design_ini(p_identifier, design_ini_path);

  QString return_sfx = "";

  if (f_result == "")
  {
    f_result = read_design_ini(p_identifier, default_path);

    if (f_result == "")
      return return_sfx;
  }

  return_sfx = f_result;

  return return_sfx;
}

QString AOApplication::get_sfx_suffix(QString sound_to_check)
{
    QString mp3_check = get_sounds_path(sound_to_check + ".mp3");
    QString opus_check = get_sounds_path(sound_to_check + ".opus");
    if (file_exists(opus_check))
    {
        return sound_to_check + ".opus";
    }
    else if (file_exists(mp3_check))
    {
        return sound_to_check + ".mp3";
    }
    return sound_to_check + ".wav";
}

QString AOApplication::get_image_suffix(QString path_to_check)
{
    QString apng_check = get_sounds_path(path_to_check + ".apng");
    if (file_exists(apng_check))
    {
        return path_to_check + ".apng";
    }
    return path_to_check + ".gif";
}


//returns whatever is to the right of "search_line =" within target_tag and terminator_tag, trimmed
//returns the empty string if the search line couldnt be found
QString AOApplication::read_char_ini(QString p_char, QString p_search_line, QString target_tag)
{
  QSettings settings(get_character_path(p_char, "char.ini"), QSettings::IniFormat);
  settings.beginGroup(target_tag);
  QString value = settings.value(p_search_line).toString();
  settings.endGroup();
  return value;
}

QString AOApplication::get_char_name(QString p_char)
{
  QString f_result = read_char_ini(p_char, "name", "Options");

  if (f_result == "")
    return p_char;
  else return f_result;
}

QString AOApplication::get_showname(QString p_char)
{
  QString f_result = read_char_ini(p_char, "showname", "Options");
  QString f_needed = read_char_ini(p_char, "needs_showname", "Options");

  if (f_needed.startsWith("false"))
    return "";
  if (f_result == "")
    return p_char;
  else return f_result;
}

QString AOApplication::get_char_side(QString p_char)
{
  QString f_result = read_char_ini(p_char, "side", "Options");

  if (f_result == "")
    return "wit";
  else return f_result;
}

QString AOApplication::get_gender(QString p_char)
{
  QString f_result = read_char_ini(p_char, "gender", "Options");

  if (f_result == "")
    return "male";
  else return f_result;
}

QString AOApplication::get_chat(QString p_char)
{
  QString f_result = read_char_ini(p_char, "chat", "Options");

  //handling the correct order of chat is a bit complicated, we let the caller do it
  return f_result;
}

QString AOApplication::get_char_shouts(QString p_char)
{
  QString f_result = read_char_ini(p_char, "shouts", "Options");

  return f_result;
}

int AOApplication::get_preanim_duration(QString p_char, QString p_emote)
{
  QString f_result = read_char_ini(p_char, p_emote, "Time");

  if (f_result == "")
    return -1;
  else return f_result.toInt();
}

int AOApplication::get_ao2_preanim_duration(QString p_char, QString p_emote)
{
  QString f_result = read_char_ini(p_char, "%" + p_emote, "Time");

  if (f_result == "")
    return -1;
  else return f_result.toInt();
}

int AOApplication::get_emote_number(QString p_char)
{
  QString f_result = read_char_ini(p_char, "number", "Emotions");

  if (f_result == "")
    return 0;
  else return f_result.toInt();
}

QString AOApplication::get_emote_comment(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "Emotions");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "normal";
  }
  else return result_contents.at(0);
}

QString AOApplication::get_pre_emote(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "Emotions");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "";
  }
  else return result_contents.at(1);
}

QString AOApplication::get_emote(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "Emotions");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "normal";
  }
  else return result_contents.at(2);
}

EMOTE_MODIFIER AOApplication::get_emote_mod(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "Emotions");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << QString::number(p_emote);
    return NO_PREANIM;
  }
  else return static_cast<EMOTE_MODIFIER>(result_contents.at(3).toInt());
}

DESK_MODIFIER AOApplication::get_desk_mod(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "Emotions");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 5)
    return DEFAULT_DESK_MOD;

  QString string_result = result_contents.at(4);
  if (string_result == "")
    return DEFAULT_DESK_MOD;

  else return static_cast<DESK_MODIFIER>(string_result.toInt());
}

QString AOApplication::get_sfx_name(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "SoundN");

  if (f_result == "")
    return "1";
  else return f_result;
}

int AOApplication::get_sfx_delay(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "SoundT");

  if (f_result == "")
    return 1;
  else return f_result.toInt();
}

int AOApplication::get_text_delay(QString p_char, QString p_emote)
{
  QString f_result = read_char_ini(p_char, p_emote, "TextDelay");

  if (f_result == "")
    return -1;
  else return f_result.toInt();
}

QString AOApplication::get_custom_realization(QString p_char)
{
  QString f_result = read_char_ini(p_char, "realization", "Options");

  if (f_result == "")
    return get_sfx("realization");
  else return f_result;
}
