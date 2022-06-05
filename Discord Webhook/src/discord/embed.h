#ifndef _DISCORD_EMBED_HEADER_
#define _DISCORD_EMBED_HEADER_

namespace wavvve::discord {
    typedef struct _Author {
        std::optional<std::string> name;
        std::optional<std::string> url;
        std::optional<std::string> icon_url;
        std::optional<std::string> proxy_icon_url;

        inline operator bool() { return name.has_value() && !( *name ).empty(); }
    } Author, *PAuthor;

    typedef struct _Field {
        std::optional<std::string> name;
        std::optional<std::string> value;
        std::optional<bool> isinline;
    } Field, *PField;

    typedef struct _Footer {
        std::optional<std::string> text;
        std::optional<std::string> icon_url;
        std::optional<std::string> proxy_icon_url;

        inline operator bool() { return text.has_value() && !( *text ).empty(); }
    } Footer, *PFooter;

    typedef struct _Image {
        std::optional<std::string> url;
        std::optional<std::string> proxy_url;
        std::optional<int> height;
        std::optional<int> width;
    } Image, *PImage;

    typedef struct _Embed {
        std::optional<std::string> type;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::optional<int64_t> color;
        std::optional<std::vector<Field>> fields;
        std::optional<std::string> timestamp;
        std::optional<Image> image;
        std::optional<Image> thumbnail;
        std::optional<Author> author;
        std::optional<Footer> footer;
        std::optional<std::string> url;

        inline operator bool() 
        { 
            return ( title.has_value() && !( *title ).empty() ) or ( description.has_value() && !( *description ).empty() ) or ( fields.has_value() && !( *fields ).empty() ) or author.has_value();
        }
    } Embed, *PEmbed;

}
#endif