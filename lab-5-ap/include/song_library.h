#ifndef SONG_LIBRARY_H
#define SONG_LIBRARY_H

#include "note_container.h"

/**
 * Song library
 */
namespace SongLibrary {

static constexpr NoteContainer MARIO[] = {
    NOTE_E3(200), NOTE_E3(200), REST(200),    NOTE_E3(200),
    REST(200),    NOTE_C3(200), NOTE_E3(200), REST(200),
    NOTE_G3(200), REST(600),    NOTE_G2(200), REST(600)};
static constexpr uint8_t MARIO_LEN = sizeof(MARIO) / sizeof(NoteContainer);

static constexpr NoteContainer MEGA[] = {
    NOTE_D3(128), NOTE_D3(128), NOTE_D4(128), REST(128),   NOTE_A3(128),
    REST(192),    NOTE_GS3(128), REST(128),     NOTE_G3(128), REST(128),
    NOTE_F3(256), NOTE_D3(128),  NOTE_F3(128),  NOTE_G3(128)};
static constexpr uint8_t MEGA_LEN = sizeof(MEGA) / sizeof(NoteContainer);

static constexpr NoteContainer SECRET[] = {
    NOTE_C3(131), NOTE_D3(131),  NOTE_F3(131), NOTE_D3(131), NOTE_A3(262),
    REST(131),    NOTE_A3(262),  REST(131),    NOTE_G3(786), NOTE_C3(131),
    NOTE_D3(131), NOTE_F3(131),  NOTE_D3(131), NOTE_G3(262), REST(131),
    NOTE_G3(262), REST(131),     NOTE_F3(524), NOTE_E3(131), NOTE_D3(262),
    NOTE_C3(131), NOTE_D3(131),  NOTE_F3(131), NOTE_D3(131), NOTE_F3(524),
    NOTE_G3(262), NOTE_E3(393),  NOTE_D3(131), NOTE_C3(524), NOTE_C3(262),
    NOTE_G3(524), NOTE_F3(1048), NOTE_C3(131), NOTE_D3(131), NOTE_F3(131),
    NOTE_D3(131), NOTE_A3(262),  REST(131),    NOTE_A3(262), REST(131),
    NOTE_G3(786), NOTE_C3(131),  NOTE_D3(131), NOTE_F3(131), NOTE_D3(131),
    NOTE_C4(524), NOTE_E3(262),  NOTE_F3(393), NOTE_E3(131), NOTE_D3(262),
    NOTE_C3(131), NOTE_D3(131),  NOTE_F3(131), NOTE_D3(131), NOTE_F3(524),
    NOTE_G3(262), NOTE_E3(393),  NOTE_D3(131), NOTE_C3(524), NOTE_C3(262),
    NOTE_G3(524), NOTE_F3(1048)};
static constexpr uint8_t SECRET_LEN = sizeof(SECRET) / sizeof(NoteContainer);

static constexpr NoteContainer STAR[] = {
    NOTE_C3(250), NOTE_C3(250), NOTE_G3(250), NOTE_G3(250),
    NOTE_A3(250), NOTE_A3(250), NOTE_G3(250), REST(250),
    NOTE_F3(250), NOTE_F3(250), NOTE_E3(250), NOTE_E3(250),
    NOTE_D3(250), NOTE_D3(250), NOTE_C3(250), REST(250)};
static constexpr uint8_t STAR_LEN = sizeof(STAR) / sizeof(NoteContainer);
}  // namespace SongLibrary

#endif  // SONG_LIBRARY_H