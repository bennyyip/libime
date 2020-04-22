/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_LIBIME_PINYIN_PINYINCONTEXT_H_
#define _FCITX_LIBIME_PINYIN_PINYINCONTEXT_H_

#include "libimepinyin_export.h"
#include <fcitx-utils/macros.h>
#include <libime/core/inputbuffer.h>
#include <libime/core/lattice.h>
#include <memory>
#include <vector>

namespace libime {
class PinyinIME;
class PinyinContextPrivate;
class LatticeNode;

class LIBIMEPINYIN_EXPORT PinyinContext : public InputBuffer {
public:
    PinyinContext(PinyinIME *ime);
    virtual ~PinyinContext();

    void setUseShuangpin(bool sp);
    bool useShuangpin() const;

    void erase(size_t from, size_t to) override;
    void setCursor(size_t pos) override;

    int maxSentenceLength() const;
    void setMaxSentenceLength(int length);

    const std::vector<SentenceResult> &candidates() const;
    void select(size_t idx);
    void cancel();
    bool cancelTill(size_t pos);

    /// Whether the input is fully selected.
    bool selected() const;

    /// The sentence for this context, can be used as preedit.
    std::string sentence() const {
        auto &c = candidates();
        if (c.size()) {
            return selectedSentence() + c[0].toString();
        } else {
            return selectedSentence();
        }
    }

    std::string preedit() const;

    /// Mixed preedit (selected hanzi + pinyin).
    std::pair<std::string, size_t> preeditWithCursor() const;

    /// Selected hanzi.
    std::string selectedSentence() const;

    /// Selected pinyin length.
    size_t selectedLength() const;

    /// Selected hanzi segments.
    std::vector<std::string> selectedWords() const;

    /// Get the full pinyin string of the selected part.
    std::string selectedFullPinyin() const;

    /// Get the full pinyin string of certain candidate.
    std::string candidateFullPinyin(size_t i) const;

    /// Add the selected part to history if selected() == true.
    void learn();

    /// Return the position of last pinyin. E.g. 你h|ao, return the offset
    /// before h.
    int pinyinBeforeCursor() const;

    /// Return the position of last pinyin. E.g. 你h|ao, return the offset after
    /// h.
    int pinyinAfterCursor() const;

    PinyinIME *ime() const;

    /// Opaque language model state.
    State state() const;

protected:
    bool typeImpl(const char *s, size_t length) override;

private:
    void update();
    bool learnWord();
    std::unique_ptr<PinyinContextPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(PinyinContext);
};
} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_PINYINCONTEXT_H_
