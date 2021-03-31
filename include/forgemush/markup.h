//
// Created by volund on 10/29/20.
//

#ifndef FORGEMUSH_MARKUP_H
#define FORGEMUSH_MARKUP_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <regex>

#define TAG_START '\002'
#define TAG_END '\003'


namespace markup {

    enum AnsiMode : uint8_t {
        Clear = 0,
        Letters = 1,
        Numbers = 2,
        Rgb = 3,
        Hex1 = 4,
        Hex2 = 5,
        Name = 6
    };

    struct AnsiGround {
        AnsiMode mode;
        uint8_t r, g, b;
        std::string text;
        bool process(std::string &mode, std::smatch &sm, bool background);
    };

    struct AnsiData {
        AnsiGround fg, bg;
        uint8_t bits, offbits;
        bool reset;
        bool parse_markup(std::string &src, std::string &errmsg);
        static std::vector<std::tuple<std::string, std::regex>> ansi_codes;
        std::vector<std::tuple<std::string, std::regex>>& get_match();
    };

    enum MarkupType : uint8_t {
        Color = 0,
        MXP = 1
    };

    struct Markup;

    typedef std::shared_ptr<Markup> ShMarkup;
    typedef std::optional<ShMarkup> OpMarkup;
    typedef std::tuple<OpMarkup, char> MarkupIdx;

    struct Markup {
        explicit Markup(char type);
        OpMarkup parent;
        MarkupType type;
        void set_parent(const ShMarkup &mark);
        std::string start_text, end_text;
        std::optional<std::unique_ptr<AnsiData>> ansi;
        bool validate(std::string &errmsg);
    };


    class MarkupString {
    public:
        MarkupString();
        MarkupString(const MarkupString &src);
        explicit MarkupString(const std::string &src);
        MarkupString operator+(const MarkupString &other);
        MarkupString& operator+=(const MarkupString &other);
        bool decode_markup(std::string &src);
    private:
        std::string text, source;
        std::vector<ShMarkup> markup;
        std::vector<MarkupIdx> idx;
        void reverse();
        void clear_idx();
        void regen_text();

    };
}



#endif //FORGEMUSH_MARKUP_H
