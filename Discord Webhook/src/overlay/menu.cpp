#include "main.h"

std::unique_ptr<wavvve::overlay::Menu> g_pMenu;

std::optional<std::string> stropt( const char* text ) {

	std::string ret = std::string( text ).c_str();

	if ( ret.empty() or !strlen(ret.c_str()))
		return std::nullopt;

	return std::make_optional<std::string>(ret);
}

struct _WebHook_No_Optional {
	char URL[4096];

	struct _Main_Data {
		char Content[2000];
		bool Tts;
		bool ActiveEmbed;
	} Main;

	struct _Embed_Data {
		char Title[256];
		char Description[4096];
		float Color[4] = { 1.f, 1.f, 1.f, 1.f };

		struct _Author_Data {
			char Name[256];
			char URL[256];
			char IconURL[256];
			char ProxyIconURL[4096];
		} Author;

		struct _Field_Data {
			char Name[256];
			char Value[1024];
			bool IsInline;
		}; 		
		std::vector<_Field_Data> Fields;

		struct _Footer_Data {
			char Text[2048];
			char IconURL[4096];
			char ProxyIconURL[4096];
		} Footer;

		struct _Image_Data {
			char URL[4096];
			char ProxyURL[4096];
			int Height = 128;
			int	Width = 128;
		} Image, Thumbnail;

		char Timestamp[256];
		char URL[4096];
	} Embed;
} WHData;

namespace wavvve::overlay {

	namespace render {
		constexpr float window_title_bar_height = 22;
		constexpr float window_under_title_bar_height = 14;

		ImDrawList *pDrawList;

		static void set_draw_list( ImDrawList *lpDrawList ) {
			if ( !pDrawList )
				pDrawList = lpDrawList;
		}

		static void draw_title_bar( ) {
			static ImU32 ColLeft, ColRight, ColOnClick;

			if ( !ColLeft )
				ColLeft = IM_COL32( 255, 59, 91, 255 );

			if ( !ColRight )
				ColRight = IM_COL32( 255, 18, 57, 255);

			if ( !ColOnClick )
				ColOnClick = IM_COL32( 237, 0, 39, 255 );

			ImVec2 Size = ImVec2( GImGui->CurrentWindow->Size.x, window_title_bar_height );

			pDrawList->AddRectFilledMultiColor( GImGui->CurrentWindow->Pos, GImGui->CurrentWindow->Pos + Size, ColLeft, ColRight, ColRight, ColLeft );

			float  BtnRadius = window_title_bar_height * 0.5f;
			ImVec2 BtnPos = ImVec2( GImGui->CurrentWindow->Size.x - BtnRadius * 4 , BtnRadius );

			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ColOnClick );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ColLeft );

			ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 212, 212, 212, 255 ) );
			ImGui::PushStyleColor( ImGuiCol_TextDisabled, IM_COL32( 242, 242, 242, 255 ) );

			ImGui::SetCursorPos( ImVec2( ImGui::GetCursorPosX() + 6, (window_title_bar_height * 0.5f) / 2 ) );
			ImGui::Text(  "Wavvve Discord Webhook"  );

			if ( ImGui::HyphenButton( GImGui->CurrentWindow->GetID( "HIDEBTN" ), BtnPos + ImVec2( BtnRadius, 0 ), BtnRadius ) )
				ShowWindow( g_pCvars->m_phWnd, SW_MINIMIZE );

			if ( ImGui::CloseButton( GImGui->CurrentWindow->GetID( "CLOSEBTN" ), BtnPos + ImVec2( BtnRadius * 3, 0 ), BtnRadius ) )
				g_pCvars->m_bClosingApp = true;

			ImGui::PopStyleColor( 4 );
		}

		static void draw_under_title_bar( ) {
			static ImU32 Col;

			if ( !Col )
				Col = IM_COL32( 255, 18, 57, 255 );  

			ImVec2 Pos = GImGui->CurrentWindow->Pos + ImVec2( 0, GImGui->CurrentWindow->Size.y - window_under_title_bar_height );
			ImVec2 Size = ImVec2( GImGui->CurrentWindow->Size.x, window_under_title_bar_height );

			pDrawList->AddRectFilled( Pos, Pos + Size, Col );

			ImGui::SetCursorPos( ImVec2( GImGui->CurrentWindow->Pos.x + 6,
				GImGui->CurrentWindow->Pos.y + GImGui->CurrentWindow->Size.y - window_under_title_bar_height ) );

			ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 212, 212, 212, 255 ) );
			ImGui::Text(  "yougame.biz" );
			ImGui::PopStyleColor();
		}

		static ImVector<ImRect> s_GroupPanelLabelStack;

		void begin_group( const char *name, const ImVec2 &size ) {
			ImGui::Spacing();
			ImGui::BeginGroup();
			
			const ImVec2 CursorPos = ImGui::GetCursorScreenPos();
			const ImVec2 ItemSpacing = ImGui::GetStyle().ItemSpacing;

			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.0f, 0.0f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.0f, 0.0f ) );

			const float FrameHeight = ImGui::GetFrameHeight();

			ImGui::BeginGroup();
			
			ImVec2 EffectiveSize = size;
			if ( size.x < 0.0f )
				EffectiveSize.x = ImGui::GetContentRegionAvail().x;
			else
				EffectiveSize.x = size.x;

			ImGui::Dummy( ImVec2( EffectiveSize.x, 0.0f ) );
			ImGui::Dummy( ImVec2( FrameHeight * 0.5f, 0.0f ) );
			ImGui::SameLine( 0.0f, 0.0f );

			ImGui::BeginGroup();
			ImGui::Dummy( ImVec2( FrameHeight * 0.5f, 0.0f ) );

			ImGui::SameLine( 0.0f, 0.0f );
			ImGui::TextUnformatted( name );

			const ImVec2 RectMin = ImGui::GetItemRectMin();
			const ImVec2 RectMax = ImGui::GetItemRectMax();

			ImGui::SameLine( 0.0f, 0.0f );
			ImGui::Dummy( ImVec2( 0.0, FrameHeight + ItemSpacing.y ) );

			ImGui::BeginGroup();

			ImGui::PopStyleVar( 2 );

		#if IMGUI_VERSION_NUM >= 17301
			ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= FrameHeight * 0.5f;
			ImGui::GetCurrentWindow()->WorkRect.Max.x -= FrameHeight * 0.5f;
			ImGui::GetCurrentWindow()->InnerRect.Max.x -= FrameHeight * 0.5f;
		#else
			ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= FrameHeight * 0.5f;
		#endif
			ImGui::GetCurrentWindow()->Size.x -= FrameHeight;

			const float ItemWidth = ImGui::CalcItemWidth();
			ImGui::PushItemWidth( ImMax( 0.0f, ItemWidth - FrameHeight ) );

			s_GroupPanelLabelStack.push_back( ImRect( RectMin, RectMax ) );
		}

		void end_group() {
			ImGui::PopItemWidth();

			const ImVec2 ItemSpacing = ImGui::GetStyle().ItemSpacing;

			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.0f, 0.0f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.0f, 0.0f ) );

			const float FrameHeight = ImGui::GetFrameHeight();

			ImGui::EndGroup();

			ImGui::EndGroup();

			ImGui::SameLine( 0.0f, 0.0f );
			ImGui::Dummy( ImVec2( FrameHeight * 0.5f, 0.0f ) );
			ImGui::Dummy( ImVec2( 0.0, FrameHeight - FrameHeight * 0.5f - ItemSpacing.y ) );

			ImGui::EndGroup();

			const ImVec2 RectMin = ImGui::GetItemRectMin();
			const ImVec2 RectMax = ImGui::GetItemRectMax();

			ImRect Rect = s_GroupPanelLabelStack.back();
			s_GroupPanelLabelStack.pop_back();

			ImVec2 HalfFrame = ImVec2( FrameHeight * 0.25f, FrameHeight ) * 0.5f;
			ImRect FrameRect = ImRect( RectMin + HalfFrame, RectMax - ImVec2( HalfFrame.x, 0.0f ) );
			Rect.Min.x -= ItemSpacing.x;
			Rect.Max.x += ItemSpacing.x;
			for ( int i = 0; i < 4; ++i ) {
				switch ( i ) {
					case 0: ImGui::PushClipRect( ImVec2( -FLT_MAX, -FLT_MAX ), ImVec2( Rect.Min.x, FLT_MAX ), true ); break;
					case 1: ImGui::PushClipRect( ImVec2( Rect.Max.x, -FLT_MAX ), ImVec2( FLT_MAX, FLT_MAX ), true ); break;
					case 2: ImGui::PushClipRect( ImVec2( Rect.Min.x, -FLT_MAX ), ImVec2( Rect.Max.x, Rect.Min.y ), true ); break;
					case 3: ImGui::PushClipRect( ImVec2( Rect.Min.x, Rect.Max.y ), ImVec2( Rect.Max.x, FLT_MAX ), true ); break;
				}

				ImGui::GetWindowDrawList()->AddRect(
					FrameRect.Min, FrameRect.Max,
					IM_COL32( 255, 59, 91, 220 ),
					4.f );

				ImGui::PopClipRect();
			}

			ImGui::PopStyleVar( 2 );

		#if IMGUI_VERSION_NUM >= 17301
			ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += FrameHeight * 0.5f;
			ImGui::GetCurrentWindow()->WorkRect.Max.x += FrameHeight * 0.5f;
			ImGui::GetCurrentWindow()->InnerRect.Max.x += FrameHeight * 0.5f;
		#else
			ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += FrameHeight * 0.5f;
		#endif
			ImGui::GetCurrentWindow()->Size.x += FrameHeight;

			ImGui::Dummy( ImVec2( 0.0f, 0.0f ) );

			ImGui::EndGroup();
			ImGui::Spacing();
		}

        namespace utils {
            void calc_window_size( ImVec2 &size ) {
                size.x = ( GImGui->CurrentWindow->Size.x - 16.f * 4.f ) * 0.5f;
                size.y = GImGui->CurrentWindow->Size.y - ImGui::GetCursorPosY() - 24.f;
            }

			int64_t create_hex_from_color( int r, int g, int b ) {
				return ( ( r & 0xff ) << 16 ) + ( ( g & 0xff ) << 8 ) + ( b & 0xff );
			}
        }
	}

	void SetupStyle() {
		static bool m_bInit = false;

		if ( !m_bInit ) {
			m_bInit = true;

			GImGui->Style.AntiAliasedFill = true;
			GImGui->Style.AntiAliasedLines = true;
			GImGui->Style.AntiAliasedLinesUseTex = true;

			GImGui->Style.ChildRounding = 4.f;
			GImGui->Style.ChildBorderSize = 1.7f;

			GImGui->Style.FrameRounding = 4.f;
			GImGui->Style.FrameBorderSize = 0.f;

			GImGui->Style.ScrollbarSize = 3.f;

			GImGui->Style.Colors[ImGuiCol_Text] = ImColor(98, 98, 98, 255);
			GImGui->Style.Colors[ImGuiCol_TextSelectedBg] = ImColor( 255, 59, 91, 170 );
			GImGui->Style.Colors[ImGuiCol_CheckMark] = ImColor( 255, 59, 91, 255 );

			GImGui->Style.Colors[ImGuiCol_Button] = ImColor( 255, 59, 91, 255 ); 
			GImGui->Style.Colors[ImGuiCol_ButtonActive] = ImColor( 237, 0, 39, 255 );
			GImGui->Style.Colors[ImGuiCol_ButtonHovered] = ImColor( 255, 18, 57, 255 );

			GImGui->Style.Colors[ImGuiCol_ScrollbarBg] = ImColor();
			GImGui->Style.Colors[ImGuiCol_ScrollbarGrab] = ImColor( 255, 59, 91, 255 );
			GImGui->Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor( 237, 0, 39, 255 );
			GImGui->Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor( 255, 18, 57, 255 );
		}
	}

	void Menu::Render() {
		SetupStyle();

		const ImVec2 BtnSize = ImVec2(100, 25);

		const ImColor BgColor = IM_COL32( 235, 235, 235, 255);

		ImGui::SetNextWindowPos( ImVec2( 0, 0 ), ImGuiCond_Once );
		ImGui::SetNextWindowSize( MENU_SIZE, ImGuiCond_Once );

		ImGui::Begin( "Wavvve Discord Webhook", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

		auto *pCurrentWindow = ImGui::GetCurrentWindow();
		auto *pDrawList = pCurrentWindow->DrawList;

		render::set_draw_list( pDrawList );

		pDrawList->PushClipRectFullScreen();

		/* draw window background */
		pDrawList->AddRectFilled( pCurrentWindow->Pos, pCurrentWindow->Size, BgColor );

		/* close, hide window */
		render::draw_title_bar( );

        ImVec2 GroupSize;

		for ( int i = 0; i < 4; i++ )
			ImGui::Spacing();

		ImGui::SameLine( 28 );

		ImGui::PushStyleColor( ImGuiCol_Border, IM_COL32( 255, 59, 91, 220 ) );
		ImGui::PushStyleColor( ImGuiCol_Separator, IM_COL32( 255, 59, 91, 220 ) );
		ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 224, 224, 224, 255 ) );

        render::utils::calc_window_size( GroupSize );
		ImGui::BeginChild("General", GroupSize, true );

		// Title
		ImGui::Spacing();
		ImGui::SetCursorPosX( 10.f );
		ImGui::Text( "GENERAL" );
		ImGui::Separator();

		// Widgets
		ImGui::Spacing();
		ImGui::SetCursorPosX( 10.f );
		ImGui::BeginGroup();
		{
			ImGui::InputText( "URL", WHData.URL, IM_ARRAYSIZE( WHData.URL ) );
			ImGui::InputText( "Content", WHData.Main.Content, IM_ARRAYSIZE( WHData.Main.Content ) );
			ImGui::Checkbox( "Text to speech", &WHData.Main.Tts );
			ImGui::Checkbox( "Use embed", &WHData.Main.ActiveEmbed );

			ImGui::SetCursorPos( ImVec2( ( ImGui::GetWindowWidth() - BtnSize.x ) * 0.5f, GImGui->CurrentWindow->Size.y - BtnSize.y - 10.f ) );

			if ( ImGui::Button( "Send", BtnSize ) ) {

				if ( !( ( std::string )WHData.URL ).empty() ) {
					if ( !( ( std::string )WHData.Main.Content ).empty() ) {
						discord::WebHook WebHook;

						WebHook.SetURL( WHData.URL );
						WebHook.WithContent( ( std::string )WHData.Main.Content ).WithTts( WHData.Main.Tts );

						if ( WHData.Main.ActiveEmbed ) {
							discord::Embed Embed;

							Embed.type = "rich";
							Embed.url = stropt( WHData.Embed.URL );
							Embed.title = stropt( WHData.Embed.Title );
							Embed.description = stropt( WHData.Embed.Description );
							Embed.color = render::utils::create_hex_from_color( ( int )( WHData.Embed.Color[0] * 255.f ), ( int )( WHData.Embed.Color[1] * 255.f ), ( int )( WHData.Embed.Color[2] * 255.f ) );

							discord::Author Author;

							Author = { stropt( WHData.Embed.Author.Name ), stropt( WHData.Embed.Author.URL ), stropt( WHData.Embed.Author.IconURL ),  stropt( WHData.Embed.Author.ProxyIconURL ) };

							if ( Author )
								Embed.author = Author;

							if ( !WHData.Embed.Fields.empty() ) {
								Embed.fields = std::vector<discord::Field>( );
								for ( auto &FieldData : WHData.Embed.Fields ) {
									discord::Field Field;

									Field.name = stropt( FieldData.Name );
									Field.value = stropt( FieldData.Value );
									Field.isinline = FieldData.IsInline;

									( *Embed.fields ).push_back( Field );
								}
							}

							Embed.image = { WHData.Embed.Image.URL, WHData.Embed.Image.ProxyURL, WHData.Embed.Image.Height, WHData.Embed.Image.Width };

							Embed.thumbnail = { WHData.Embed.Thumbnail.URL, WHData.Embed.Thumbnail.ProxyURL, WHData.Embed.Thumbnail.Height, WHData.Embed.Thumbnail.Width };

							discord::Footer Footer;
							Footer = { stropt( WHData.Embed.Footer.Text ), stropt( WHData.Embed.Footer.IconURL ), stropt( WHData.Embed.Footer.ProxyIconURL ) };

							if ( Footer )
								Embed.footer = Footer;

							Embed.timestamp = stropt( WHData.Embed.Timestamp );

							if ( Embed )
								WebHook.WithEmbeds( std::vector<discord::Embed> { Embed } );
						}

						WebHook.Send();
					}
				}
			}
		}
		ImGui::EndGroup();

		ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild( "Embed", GroupSize, true );

		// Title
		ImGui::Spacing();
		ImGui::SetCursorPosX( 10.f );
		ImGui::Text( "EMBED" );
		ImGui::Separator();

		// Widgets
		ImGui::Spacing();
		ImGui::SetCursorPosX( 10.f );
		ImGui::BeginGroup();
		{
			ImGui::InputText( "URL##EMBED", WHData.Embed.URL, IM_ARRAYSIZE( WHData.Embed.URL ) );
			ImGui::InputText( "Title", WHData.Embed.Title, IM_ARRAYSIZE( WHData.Embed.Title ) );
			ImGui::InputText( "Description", WHData.Embed.Description, IM_ARRAYSIZE( WHData.Embed.Description ) );
			ImGui::ColorEdit4( "Color", WHData.Embed.Color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs );

			render::begin_group( "Author", ImVec2( GImGui->CurrentWindow->Size.x - 20.f, 0 ) );
			{
				ImGui::InputText( "Name##AUTHOR", WHData.Embed.Author.Name, IM_ARRAYSIZE( WHData.Embed.Author.Name ) );
				ImGui::InputText( "URL##AUTHOR", WHData.Embed.Author.URL, IM_ARRAYSIZE( WHData.Embed.Author.URL ) );
				ImGui::InputText( "Icon URL##AUTHOR", WHData.Embed.Author.IconURL, IM_ARRAYSIZE( WHData.Embed.Author.IconURL ) );
				ImGui::InputText( "Proxy URL##AUTHOR", WHData.Embed.Author.ProxyIconURL, IM_ARRAYSIZE( WHData.Embed.Author.ProxyIconURL ) );

				ImGui::Spacing();
			}
			render::end_group();

			if ( WHData.Embed.Fields.size() < 25 ) 
			{
				if ( ImGui::Button( "Add field", BtnSize ) )
					WHData.Embed.Fields.push_back( {} );
			}

			if ( !WHData.Embed.Fields.empty() ) 
			{
				if ( WHData.Embed.Fields.size() < 25 )
					ImGui::SameLine();

				if ( ImGui::Button( "Remove all fields", BtnSize ) )
					WHData.Embed.Fields.clear();

				render::begin_group( "Fields", ImVec2( GImGui->CurrentWindow->Size.x - 20.f, 0 ) );
				{
					for ( size_t i = 0; i < WHData.Embed.Fields.size(); i++ ) {
						ImGui::InputText( ( "Name##Field" + std::to_string( i ) ).c_str(), WHData.Embed.Fields[i].Name, IM_ARRAYSIZE( WHData.Embed.Fields[i].Name ) );
						ImGui::InputText( ( "Value##Field" + std::to_string( i ) ).c_str(), WHData.Embed.Fields[i].Value, IM_ARRAYSIZE( WHData.Embed.Fields[i].Value ) );
						ImGui::Checkbox( ( "Inline##Field" + std::to_string( i ) ).c_str(), &WHData.Embed.Fields[i].IsInline );

						ImGui::SameLine();

						if ( ImGui::Button( ( "Remove##Field" + std::to_string( i ) ).c_str() ) )
							WHData.Embed.Fields.erase( WHData.Embed.Fields.begin() + i );
					}
					ImGui::Spacing();
				}
				render::end_group();
			}

			render::begin_group( "Image", ImVec2( GImGui->CurrentWindow->Size.x - 20.f, 0 ) );
			{
				ImGui::InputText( "URL##IMAGE" , WHData.Embed.Image.URL, IM_ARRAYSIZE( WHData.Embed.Image.URL ) );
				ImGui::InputText( "Proxy URL##IMAGE", WHData.Embed.Image.ProxyURL, IM_ARRAYSIZE( WHData.Embed.Image.ProxyURL ) );
				ImGui::InputInt( "Width##IMAGE", &WHData.Embed.Image.Width );
				ImGui::InputInt( "Height##IMAGE", &WHData.Embed.Image.Height );
				ImGui::Spacing();
			}
			render::end_group();

			render::begin_group( "Thumbnail", ImVec2( GImGui->CurrentWindow->Size.x - 20.f, 0 ) );
			{
				ImGui::InputText( "URL##THUMBNAIL", WHData.Embed.Thumbnail.URL, IM_ARRAYSIZE( WHData.Embed.Thumbnail.URL ) );
				ImGui::InputText( "Proxy URL##THUMBNAIL", WHData.Embed.Thumbnail.ProxyURL, IM_ARRAYSIZE( WHData.Embed.Thumbnail.ProxyURL ) );
				ImGui::InputInt( "Width##THUMBNAIL", &WHData.Embed.Thumbnail.Width );
				ImGui::InputInt( "Height##THUMBNAIL", &WHData.Embed.Thumbnail.Height );
				ImGui::Spacing();
			}
			render::end_group();

			render::begin_group( "Footer", ImVec2( GImGui->CurrentWindow->Size.x - 20.f, 0 ) );
			{
				ImGui::InputText( "Text##FOOTER", WHData.Embed.Footer.Text, IM_ARRAYSIZE( WHData.Embed.Footer.Text ) );
				ImGui::InputText( "Icon URL##FOOTER", WHData.Embed.Footer.IconURL, IM_ARRAYSIZE( WHData.Embed.Footer.IconURL ) );
				ImGui::InputText( "Proxy URL##FOOTER", WHData.Embed.Footer.ProxyIconURL, IM_ARRAYSIZE( WHData.Embed.Footer.ProxyIconURL ) );
				ImGui::Spacing();
			}
			render::end_group();

			ImGui::InputText( "Timestamp", WHData.Embed.Timestamp, IM_ARRAYSIZE( WHData.Embed.Timestamp ) );
			ImGui::Spacing();
		}
		ImGui::EndGroup();

		ImGui::EndChild();

		ImGui::PopStyleColor(3);

		render::draw_under_title_bar( );

		pDrawList->PopClipRect();

		ImGui::End();
	}
}