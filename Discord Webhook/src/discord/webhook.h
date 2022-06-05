#ifndef _DISCORD_WEBHOOK_HEADER_
#define _DISCORD_WEBHOOK_HEADER_

namespace wavvve::discord {

    class WebHook {
    private:
        std::string m_sUrl;
    public:
        void SetURL(const std::string url);

        WebHook &WithContent( const std::string content );
        WebHook &WithTts( const bool tts );
        WebHook &WithEmbeds( const std::vector<Embed> embeds );

        WebHook &WithContent( const std::optional<std::string> content );
        WebHook &WithTts( const std::optional<bool> tts );
        WebHook &WithEmbeds( const std::optional<std::vector<Embed>> embeds );

        std::string Send() const;

        std::optional<std::string> GetContent() const;
        std::optional<bool> GetTts() const;
        std::optional<std::vector<Embed>> GetEmbeds() const;
    private:
        std::optional<std::string> m_Content;
        std::optional<bool> m_Tts;
        std::optional<std::vector<Embed>> m_Embeds;
    };
}

#endif