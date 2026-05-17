#ifndef SPELL_CHECKER_HIGHLIGHTER_H
#define SPELL_CHECKER_HIGHLIGHTER_H

#include "spell_checker.h"

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class spell_checker_highlighter : public QSyntaxHighlighter {
public:
    explicit spell_checker_highlighter(spell_checker* checker, QTextDocument* parent)
        : QSyntaxHighlighter(parent)
        , checker(checker)
    {
        misspelled_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
        misspelled_format.setUnderlineColor(Qt::red);
    }

protected:
    void highlightBlock(const QString& text) override
    {
        static QRegularExpression word_pattern("\\b[a-zA-Z]+\\b");
        auto it = word_pattern.globalMatch(text);

        while (it.hasNext()) {
            const auto match = it.next();
            const std::string word = match.captured().toStdString();
            if (!checker->is_correct(word)) {
                setFormat(match.capturedStart(), match.capturedLength(), misspelled_format);
            }
        }
    }

private:
    spell_checker* checker;
    QTextCharFormat misspelled_format;
};

#endif // SPELL_CHECKER_HIGHLIGHTER_H
