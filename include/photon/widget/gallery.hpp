/*=================================================================================================
   Copyright (c) 2016 Joel de Guzman

   Licensed under a Creative Commons Attribution-ShareAlike 4.0 International.
   http://creativecommons.org/licenses/by-sa/4.0/
=================================================================================================*/
#if !defined(PHOTON_GUI_LIB_WIDGET_GALLERY_JUNE_5_2016)
#define PHOTON_GUI_LIB_WIDGET_GALLERY_JUNE_5_2016

#include <photon/support/theme.hpp>
#include <photon/support/text_utils.hpp>
#include <photon/widget.hpp>

namespace photon
{
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Frames
   ////////////////////////////////////////////////////////////////////////////////////////////////
   class frame : public widget
   {
   public:

      virtual void   draw(context const& ctx);
   };

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Headings
   ////////////////////////////////////////////////////////////////////////////////////////////////
   class heading : public widget
   {
   public:

                              heading(std::string const& text, float size_ = 1.0)
                               : _text(text)
                               , _size(size_)
                              {}

      virtual widget_limits   limits(basic_context const& ctx) const;
      virtual void            draw(context const& ctx);

      std::string             text() const                     { return _text; }
      void                    text(std::string const& text)    { _text = text; }

      using widget::text;

   private:

      std::string       _text;
      float             _size;
   };

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Labels
   ////////////////////////////////////////////////////////////////////////////////////////////////
   class label : public widget
   {
   public:

                              label(std::string const& text, float size_ = 1.0)
                               : _text(text)
                               , _size(size_)
                              {}

      virtual widget_limits   limits(basic_context const& ctx) const;
      virtual void            draw(context const& ctx);

      std::string             text() const                     { return _text; }
      void                    text(std::string const& text)    { _text = text; }

      using widget::text;

   private:

      std::string       _text;
      float             _size;
   };

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Grid Lines
   ////////////////////////////////////////////////////////////////////////////////////////////////
   class vgrid_lines : public widget
   {
   public:

                              vgrid_lines(float major_divisions, float minor_divisions)
                               : _major_divisions(major_divisions)
                               , _minor_divisions(minor_divisions)
                              {}

      virtual void            draw(context const& ctx);

   private:

      float                   _major_divisions;
      float                   _minor_divisions;
   };

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Groups
   ////////////////////////////////////////////////////////////////////////////////////////////////
   template <typename Heading, typename Content>
   inline auto make_group(
      Heading&&   heading,
      Content&&   content,
      bool        center_heading = true
   )
   {
      auto align_ = center_heading? 0.5 : 0;
      return
        layer(
            align_top(halign(align_, margin({ 10, 4, 10, 4 }, heading))),
            std::forward<Content>(content),
            frame{}
        );
   }

   template <typename Content>
   inline auto group(
      std::string    title,
      Content&&      content,
      float          label_size = 1.0,
      bool           center_heading = true
   )
   {
      return make_group(
         left_top_margin({ 10, 10 }, heading{ title, label_size }),
         std::forward<Content>(content), center_heading
      );
   }

   template <typename Heading, typename Content>
   inline auto make_unframed_group(
      Heading&&   heading,
      Content&&   content,
      bool        center_heading = true
   )
   {
      auto align_ = center_heading? 0.5 : 0;
      return
        layer(
            align_top(halign(align_, margin({ 10, 4, 10, 4 }, heading))),
            std::forward<Content>(content)
        );
   }

   template <typename Content>
   inline auto unframed_group(
      std::string    title,
      Content&&      content,
      float          label_size = 1.0,
      bool           center_heading = true
   )
   {
      return make_unframed_group(
         left_top_margin({ 10, 10 }, heading{ title, label_size }),
         std::forward<Content>(content), center_heading
      );
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Captions
   ////////////////////////////////////////////////////////////////////////////////////////////////
   template <typename Content>
   inline auto caption(Content&& content, std::string const& title, float size = 1.0)
   {
      return
         vtile(
            std::forward<Content>(content),
            align_center(top_margin(5.0, label(title, size)))
         );
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Icons
   ////////////////////////////////////////////////////////////////////////////////////////////////
   class icon : public widget
   {
   public:
                              icon(std::uint32_t code_, float size_ = -1);

      virtual widget_limits   limits(basic_context const& ctx) const;
      virtual void            draw(context const& ctx);

   private:

      std::uint32_t           _code;
      float                   _size;
   };

   //////////////////////////////////////////////////////////////////////////////////////////////////
   //// Buttons
   //////////////////////////////////////////////////////////////////////////////////////////////////
   auto const button_margin = rect{ 10, 5, 10, 5 };
   auto const default_button_color = color{ 0, 0, 0, 0 };

   class basic_button_body : public widget
   {
   public:

      static float corner_radius;

                     basic_button_body(color body_color);
      virtual void   draw(context const& ctx);

   private:

      color          body_color;
   };

   inline basic_button_body::basic_button_body(color body_color)
    : body_color(body_color)
   {}

   template <typename Button, typename Label>
   inline Button make_button(Label&& label, color body_color = default_button_color)
   {
      auto btn_body_off = basic_button_body(body_color.level(0.9));
      auto btn_body_on = basic_button_body(body_color.opacity(0.5));

      auto btn_img_off = layer(label, btn_body_off);
      auto btn_img_on = left_top_margin({1, 1}, layer(label, btn_body_on));

      return Button(std::move(btn_img_off), std::move(btn_img_on));
   }

   template <typename Button>
   inline Button make_button(
      std::string const& text
    , color body_color = default_button_color
   )
   {
      return make_button<Button>(
         margin(
            button_margin,
            align_center(label(text, 0.8))
         ),
         body_color
      );
   }

   template <typename Button>
   inline Button make_button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color = default_button_color
   )
   {
      return make_button<Button>(
         margin(
            button_margin,
            align_center(
               htile(
                  right_margin(8, icon(icon_code)),
                  label(text, 0.8)
               )
            )
         ),
         body_color
      );
   }

   template <typename Button>
   inline Button make_button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color = default_button_color
   )
   {
      return make_button<Button>(
         margin(
            button_margin,
            align_center(
               htile(
                  label(text, 0.8),
                  left_margin(8, icon(icon_code))
               )
            )
         ),
         body_color
      );
   }

   basic_button
   button(
      std::string const& text
    , color body_color = default_button_color
   );

   basic_button
   button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color = default_button_color
   );

   basic_button
   button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color = default_button_color
   );

   basic_toggle_button
   toggle_button(
      std::string const& text
    , color body_color = default_button_color
   );

   basic_toggle_button
   toggle_button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color = default_button_color
   );

   basic_toggle_button
   toggle_button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color = default_button_color
   );

   basic_latching_button
   latching_button(
      std::string const& text
    , color body_color = default_button_color
   );

   basic_latching_button
   latching_button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color = default_button_color
   );

   basic_latching_button
   latching_button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color = default_button_color
   );

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Check Box
   ////////////////////////////////////////////////////////////////////////////////////////////////
   void draw_check_box(
      context const& ctx, std::string const& text, bool state, bool hilite
   );

   template <bool state>
   class check_box_widget : public widget
   {
   public:
                              check_box_widget(std::string const& text)
                               : _text(text)
                              {}

      virtual widget_limits   limits(basic_context const& ctx) const;
      virtual void            draw(context const& ctx);
      virtual bool            cursor(context const& ctx, point p, cursor_tracking status);

   private:

      std::string             _text;
      bool                    _hit = false;
   };

   template <bool state>
   widget_limits check_box_widget<state>::limits(basic_context const& ctx) const
   {
      auto const&    theme_ = get_theme();
      auto&          canvas_ = ctx.canvas;

      canvas_.font(theme_.label_font, theme_.label_font_size);
      auto  info = canvas_.measure_text(_text.c_str());
      auto  height = info.ascent + info.descent + info.leading;
      return { { info.size.x, height }, { info.size.x, height } };
   }

   template <bool state>
   void check_box_widget<state>::draw(context const& ctx)
   {
      draw_check_box(ctx, _text, state, _hit);
   }

   inline basic_toggle_button check_box(std::string const& text)
   {
      return basic_toggle_button(
         check_box_widget<false>{ text }
       , check_box_widget<true>{ text }
      );
   }

   template <bool state>
   bool check_box_widget<state>::cursor(context const& ctx, point p, cursor_tracking status)
   {
      _hit = (status != cursor_tracking::leaving) && ctx.bounds.includes(p);
      widget::cursor(ctx, p, status);
      return _hit;
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Icon Button
   ////////////////////////////////////////////////////////////////////////////////////////////////
   void draw_icon_button(context const& ctx, uint32_t code, float size, bool state, bool hilite);

   template <bool state>
   class icon_button_widget : public widget
   {
   public:
                              icon_button_widget(uint32_t code, float size)
                               : _code(code)
                               , _size(size)
                              {}

      virtual widget_limits   limits(basic_context const& ctx) const;
      virtual void            draw(context const& ctx);
      virtual bool            cursor(context const& ctx, point p, cursor_tracking status);

      uint32_t                _code;
      float                   _size;
      bool                    _hit = false;
   };

   template <bool state>
   widget_limits icon_button_widget<state>::limits(basic_context const& ctx) const
   {
      auto  size = _size * 1.8f;
      return { { size, size }, { size, size } };
   }

   template <bool state>
   void icon_button_widget<state>::draw(context const& ctx)
   {
      draw_icon_button(ctx, _code, _size, state, _hit);
   }

   template <bool state>
   bool icon_button_widget<state>::cursor(context const& ctx, point p, cursor_tracking status)
   {
      _hit = (status != cursor_tracking::leaving) && ctx.bounds.includes(p);
      widget::cursor(ctx, p, status);
      return _hit;
   }

   inline basic_toggle_button icon_button(uint32_t code, float size)
   {
      return {
         icon_button_widget<false>{ code, size }
       , icon_button_widget<true>{ code, size }
      };
   }

   inline basic_toggle_button icon_button(uint32_t code1, uint32_t code2, float size)
   {
      return {
         icon_button_widget<false>{ code1, size }
       , icon_button_widget<true>{ code2, size }
      };
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Dropdown Menu
   ////////////////////////////////////////////////////////////////////////////////////////////////
   basic_dropdown_menu
   dropdown_menu(
      std::string const& text
    , color body_color = default_button_color
   );

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Menu Items
   ////////////////////////////////////////////////////////////////////////////////////////////////
   inline auto menu_item_text(std::string const& text)
   {
      return xside_margin({ 20, 20 }, align_left(label(text, 0.8)));
   }

   inline auto menu_item(std::string const& text)
   {
      return basic_menu_item(menu_item_text(text));
   }

   class menu_item_spacer_widget : public widget
   {
   public:

      virtual widget_limits limits(basic_context const& ctx) const;
      virtual void          draw(context const& ctx);
   };

   inline auto menu_item_spacer()
   {
      return menu_item_spacer_widget{};
   }
}

#endif