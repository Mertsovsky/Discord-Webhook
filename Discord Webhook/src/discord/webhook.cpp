#include "main.h"

using nlohmann::json;

#ifndef NLOHMANN_OPTIONAL
#define NLOHMANN_OPTIONAL
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json( json &j, const std::optional<T> &opt ) {
            if ( !opt.has_value() ) j = nullptr; else j = *opt;
        }

        static std::optional<T> from_json( const json &j ) {
            if ( j.is_null() ) return std::nullopt; else return j.get<T>();
        }
    };
}
#endif

namespace wavvve::optional {
    template <typename T>
    std::optional<T> Get( const json &j, const char *property ) {
        if ( j.contains( property ) )
            return j.at( property ).get<T>();
        return std::nullopt;
    }

    template <typename T>
    std::optional<T> Get( const json &j, std::string property ) {
        return Get<T>( j, property.data() );
    }
}

namespace nlohmann {
    void from_json( const json &j, wavvve::discord::Author &x ) {
        x.name = wavvve::optional::Get<std::string>( j, "name" );
        x.url = wavvve::optional::Get<std::string>( j, "url" );
        x.icon_url = wavvve::optional::Get<std::string>( j, "icon_url" );
        x.proxy_icon_url = wavvve::optional::Get<std::string>( j, "proxy_icon_url" );
    }

    void to_json( json &j, const wavvve::discord::Author &x ) {
        j = json::object();
        j["name"] = x.name;
        j["url"] = x.url;
        j["icon_url"] = x.icon_url;
        j["proxy_icon_url"] = x.proxy_icon_url;
    }

    void from_json( const json &j, wavvve::discord::Field &x ) {
        x.name = wavvve::optional::Get<std::string>( j, "name" );
        x.value = wavvve::optional::Get<std::string>( j, "value" );
        x.isinline = wavvve::optional::Get<bool>( j, "inline" );
    }

    void to_json( json &j, const wavvve::discord::Field &x ) {
        j = json::object();
        j["name"] = x.name;
        j["value"] = x.value;
        j["inline"] = x.isinline;
    }

    void from_json( const json &j, wavvve::discord::Footer &x ) {
        x.text = wavvve::optional::Get<std::string>( j, "text" );
        x.icon_url = wavvve::optional::Get<std::string>( j, "icon_url" );
        x.proxy_icon_url = wavvve::optional::Get<std::string>( j, "proxy_icon_url" );
    }

    void to_json( json &j, const wavvve::discord::Footer &x ) {
        j = json::object();
        j["text"] = x.text;
        j["icon_url"] = x.icon_url;
        j["proxy_icon_url"] = x.proxy_icon_url;
    }

    void from_json( const json &j, wavvve::discord::Image &x ) {
        x.url = wavvve::optional::Get<std::string>( j, "url" );
        x.proxy_url = wavvve::optional::Get<std::string>( j, "proxy_url" );
        x.height = wavvve::optional::Get<int>( j, "height" );
        x.width = wavvve::optional::Get<int>( j, "width" );
    }

    void to_json( json &j, const wavvve::discord::Image &x ) {
        j = json::object();
        j["url"] = x.url;
        j["proxy_url"] = x.proxy_url;
        j["height"] = x.height;
        j["width"] = x.width;
    }

    void from_json( const json &j, wavvve::discord::Embed &x ) {
        x.type = wavvve::optional::Get<std::string>( j, "type" );
        x.title = wavvve::optional::Get<std::string>( j, "title" );
        x.description = wavvve::optional::Get<std::string>( j, "description" );
        x.color = wavvve::optional::Get<int64_t>( j, "color" );
        x.fields = wavvve::optional::Get<std::vector<wavvve::discord::Field>>( j, "fields" );
        x.timestamp = wavvve::optional::Get<std::string>( j, "timestamp" );
        x.image = wavvve::optional::Get<wavvve::discord::Image>( j, "image" );
        x.thumbnail = wavvve::optional::Get<wavvve::discord::Image>( j, "thumbnail" );
        x.author = wavvve::optional::Get<wavvve::discord::Author>( j, "author" );
        x.footer = wavvve::optional::Get<wavvve::discord::Footer>( j, "footer" );
        x.url = wavvve::optional::Get<std::string>( j, "url" );
    }

    void to_json( json &j, const wavvve::discord::Embed &x ) {
        j = json::object();
        j["type"] = x.type;
        j["title"] = x.title;
        j["description"] = x.description;
        j["color"] = x.color;
        j["fields"] = x.fields;
        j["timestamp"] = x.timestamp;
        j["image"] = x.image;
        j["thumbnail"] = x.thumbnail;
        j["author"] = x.author;
        j["footer"] = x.footer;
        j["url"] = x.url;
    }

    void from_json( const json &j, wavvve::discord::WebHook &x ) {

        x.WithContent( wavvve::optional::Get<std::string>( j, "content" ) );
        x.WithTts( wavvve::optional::Get<bool>( j, "tts" ) );
        x.WithEmbeds( wavvve::optional::Get<std::vector<wavvve::discord::Embed>>( j, "embeds" ) );
    }

    void to_json( json &j, const wavvve::discord::WebHook &x ) {
        j = json::object();
        j["content"] = x.GetContent();
        j["tts"] = x.GetTts();
        j["embeds"] = x.GetEmbeds();
    }
}

namespace wavvve::url {
    void clear_domain(std::string& url, const std::vector<std::string> domains) {
        static std::vector<std::string> vProtocols = { "http://", "https://", "www." };

        for ( auto &Protocol : vProtocols )
            if ( url.starts_with( Protocol ) )
                Helper::StringReplace( url, Protocol, "" );

        for ( auto &Domain : domains )
            Helper::StringReplace( url, Domain, "" );
    }
}

namespace wavvve::discord {
    void WebHook::SetURL( const std::string url ) {
        m_sUrl = url;
    }

    WebHook &WebHook::WithContent( const std::string content ) {
        m_Content = content;
        return *this;
    }

    WebHook &WebHook::WithTts( const bool tts ) {
        m_Tts = tts;
        return *this;
    }

    WebHook &WebHook::WithEmbeds( const std::vector<Embed> embeds ) {
        m_Embeds = embeds;
        return *this;
    }

    WebHook &WebHook::WithContent( const std::optional<std::string> content ) {
        m_Content = content;
        return *this;
    }

    WebHook &WebHook::WithTts( const std::optional<bool> tts ) {
        m_Tts = tts;
        return *this;
    }

    WebHook &WebHook::WithEmbeds( const std::optional<std::vector<Embed>> embeds ) {
        m_Embeds = embeds;
        return *this;
    }

    std::string WebHook::Send() const {
        static std::vector<std::string> vDomains = { "discordapp.com", "discord.gg", "discord.com", "discord.media"};

        json j;
        nlohmann::to_json( j, *this );

        std::string sPath = m_sUrl;
        wavvve::url::clear_domain( sPath, vDomains );

        const std::string result = Helper::InternetPOSTRequest( vDomains[0], sPath, j.dump(), "Content-Type: application/json");
        return result;
    }

    std::optional<std::string> WebHook::GetContent() const {
        return m_Content;
    }

    std::optional<bool> WebHook::GetTts() const {
        return m_Tts;
    }

    std::optional<std::vector<Embed>> WebHook::GetEmbeds() const {
        return m_Embeds;
    }
}