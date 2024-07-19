#include "wrap.h"

#include <ydb-cpp-sdk/util/string/escape.h>

#include <src/library/testing/unittest/registar.h>

Y_UNIT_TEST_SUITE(Wrap) {
    Y_UNIT_TEST(TestWrapping) {
        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b c d eeffeff")),
            NUtils::Quote(std::string("a b c\nd\neeffeff"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\nc d\neeffeff")),
            NUtils::Quote(std::string("a b\nc d\neeffeff"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\n     c d\neeffeff")),
            NUtils::Quote(std::string("a b\n     c\nd\neeffeff"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\nx     c d\neeffeff")),
            NUtils::Quote(std::string("a b\nx\nc d\neeffeff"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\nx  \n   c d\neeffeff")),
            NUtils::Quote(std::string("a b\nx\n   c\nd\neeffeff"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\nx      \n   c d\neeffeff")),
            NUtils::Quote(std::string("a b\nx\n   c\nd\neeffeff"))
        );
    }

    Y_UNIT_TEST(TestWrappingIndent) {
        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b c d", "|>")),
            NUtils::Quote(std::string("a b\n|>c d"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b\n\nc d", "|>")),
            NUtils::Quote(std::string("a b\n|>\n|>c d"))
        );
    }

    Y_UNIT_TEST(TestWrappingAnsi) {
        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5m")),
            NUtils::Quote(std::string("\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5mx\033[1;2;3;4;5m"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a \033[1;2;3;4;5mb c\033[1;2;3;4;5m \033[1;2;3;4;5md e f")),
            NUtils::Quote(std::string("a \033[1;2;3;4;5mb c\033[1;2;3;4;5m\n\033[1;2;3;4;5md e f"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b  \033[1;2;3;4;5m  c d")),
            NUtils::Quote(std::string("a b  \033[1;2;3;4;5m\nc d"))
        );

        UNIT_ASSERT_STRINGS_EQUAL(
            NUtils::Quote(NLastGetopt::Wrap(5, "a b       \033[1;2;3;4;5m  c d")),
            NUtils::Quote(std::string("a b\n\033[1;2;3;4;5m  c d"))
        );
    }

    Y_UNIT_TEST(TestTextInfo) {
        size_t lastLineLen;
        bool hasParagraphs;

        NLastGetopt::Wrap(5, "a b c d e", "", &lastLineLen, &hasParagraphs);
        UNIT_ASSERT_VALUES_EQUAL(lastLineLen, 3);
        UNIT_ASSERT_VALUES_EQUAL(hasParagraphs, false);

        NLastGetopt::Wrap(5, "a b c\n\nd e f h", "", &lastLineLen, &hasParagraphs);
        UNIT_ASSERT_VALUES_EQUAL(lastLineLen, 1);
        UNIT_ASSERT_VALUES_EQUAL(hasParagraphs, true);

        NLastGetopt::Wrap(5, "a b c\n\n", "", &lastLineLen, &hasParagraphs);
        UNIT_ASSERT_VALUES_EQUAL(lastLineLen, 0);
        UNIT_ASSERT_VALUES_EQUAL(hasParagraphs, true);

        NLastGetopt::Wrap(5, "\n \na b c", "", &lastLineLen, &hasParagraphs);
        UNIT_ASSERT_VALUES_EQUAL(lastLineLen, 5);
        UNIT_ASSERT_VALUES_EQUAL(hasParagraphs, true);

        NLastGetopt::Wrap(5, "\nx\na b c", "", &lastLineLen, &hasParagraphs);
        UNIT_ASSERT_VALUES_EQUAL(lastLineLen, 5);
        UNIT_ASSERT_VALUES_EQUAL(hasParagraphs, false);
    }
}
