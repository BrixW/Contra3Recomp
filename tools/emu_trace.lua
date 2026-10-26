--[[
  Contra III: The Alien Wars (USA) - Reference Emulator Trace Exporter

  Produces a per-frame CSV with exactly the same columns as
      contra3.exe --headless 0 --movie <movie> --trace ours.csv
  so tools/compare_trace.py can check the static recompilation against a
  cycle-accurate emulator, frame by frame.

  Supported hosts:
    * BizHawk 2.x (bsnes / Snes9x cores): Tools > Lua Console, open this script
      after loading the ROM and the movie (play from power-on).
    * Snes9x-rr 1.43+/1.51 Lua: File > Lua Scripting > New Lua Script Window.

  The "lag" column is the emulator's lag flag (the game did not poll the joypad
  that frame). Pass this CSV to contra3.exe with --lag-from so lag frames of the
  movie are skipped on playback (the recompilation never lags).

  Keep the WRAM probe addresses in sync with PROBE_* in src/win32_main.c.
]]

local OUT_PATH = "emu_trace.csv"

local PROBE = {
  state    = 0x0032, sub   = 0x0034, frame_ctr = 0x003A, nmi_mode = 0x0042,
  rng_lo   = 0x00E8, rng_hi = 0x00EA,
  p1_slot  = 0x0200, p2_slot = 0x0240, slot_x = 0x06, slot_y = 0x0E,
  cam_x    = 0x1380, stage = 0x0086, lives1 = 0x1F8A, lives2 = 0x1FCA,
}

-- $4218 auto-joypad bit layout
local BTN = {
  B = 0x8000, Y = 0x4000, Select = 0x2000, Start = 0x1000,
  Up = 0x0800, Down = 0x0400, Left = 0x0200, Right = 0x0100,
  A = 0x0080, X = 0x0040, L = 0x0020, R = 0x0010,
}

local is_bizhawk = (bizstring ~= nil) or (client ~= nil and client.getversion ~= nil)

local r8, r16, frame_count, is_lag, pad_mask, movie_active

if is_bizhawk then
  memory.usememorydomain("WRAM")
  r8  = function(a) return memory.read_u8(a, "WRAM") end
  r16 = function(a) return memory.read_u16_le(a, "WRAM") end
  frame_count = function() return emu.framecount() end
  is_lag = function() return emu.islagged() end
  pad_mask = function(port)
    local inp = movie.isloaded() and movie.getinput(emu.framecount() - 1) or joypad.get()
    local m = 0
    for name, bit in pairs(BTN) do
      if inp["P" .. port .. " " .. name] then m = m + bit end
    end
    return m
  end
  movie_active = function()
    return movie.isloaded() and movie.mode() ~= "FINISHED" and emu.framecount() < movie.length()
  end
else
  -- Snes9x-rr
  r8  = function(a) return memory.readbyte(0x7E0000 + a) end
  r16 = function(a) return memory.readword(0x7E0000 + a) end
  frame_count = function() return emu.framecount() end
  is_lag = function() return emu.lagged() end
  pad_mask = function(port)
    local inp = joypad.get(port)
    local m = 0
    for name, bit in pairs(BTN) do
      if inp[name] or inp[string.lower(name)] then m = m + bit end
    end
    return m
  end
  movie_active = function() return movie.active() and movie.mode() ~= "finished" end
end

local out = assert(io.open(OUT_PATH, "w"))
out:write("frame,movie_frame,pad1,pad2,state,sub,nmi_mode,stage,frame_ctr,",
          "p1_x,p1_y,p2_x,p2_y,lives1,lives2,cam_x,rng_lo,rng_hi,lag\n")

local rows = 0
while true do
  emu.frameadvance()
  local f = frame_count()
  out:write(string.format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
    f, f - 1, pad_mask(1), pad_mask(2),
    r8(PROBE.state), r8(PROBE.sub), r8(PROBE.nmi_mode), r16(PROBE.stage), r16(PROBE.frame_ctr),
    r16(PROBE.p1_slot + PROBE.slot_x), r16(PROBE.p1_slot + PROBE.slot_y),
    r16(PROBE.p2_slot + PROBE.slot_x), r16(PROBE.p2_slot + PROBE.slot_y),
    r16(PROBE.lives1), r16(PROBE.lives2), r16(PROBE.cam_x),
    r16(PROBE.rng_lo), r16(PROBE.rng_hi),
    is_lag() and 1 or 0))
  rows = rows + 1
  if rows % 3600 == 0 then out:flush() end
  if not movie_active() then break end
end
out:close()
print(string.format("emu_trace: wrote %d frames to %s", rows, OUT_PATH))
