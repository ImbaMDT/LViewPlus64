from LViewPlus64 import *
from commons.utils import *
from commons.skills import *
from commons.items import *
from commons.targeting import *
import json, time, math

LViewPlus64_script_info = {
    "script": "WS+ Jinx",
    "author": "bckd00r",
    "description": "WS+ Jinx",
    "target_champ": "jinx",
}

FishStacks = 0
isFishBones = True

combo_key = 57
harass_key = 45
killsteal_key = 46
laneclear_key = 47
flee_key = 30

use_q_in_combo = True
use_w_in_combo = True
use_e_in_combo = True
use_r_in_combo = True

steal_kill_with_w = False
steal_kill_with_r = False

draw_q_range = False
draw_w_range = False
draw_e_range = False
draw_r_range = False

laneclear_with_q = True

w = {"Range": 1400}
e = {"Range": 900}


def LViewPlus64_load_cfg(cfg):
    global combo_key, harass_key, laneclear_key, killsteal_key, flee_key
    global use_q_in_combo, use_w_in_combo, use_e_in_combo, use_r_in_combo
    global steal_kill_with_w, steal_kill_with_r
    global draw_q_range, draw_w_range, draw_e_range, draw_r_range
    global laneclear_with_q

    combo_key = cfg.get_int("combo_key", 57)
    harass_key = cfg.get_int("harass_key", 45)
    killsteal_key = cfg.get_int("killsteal_key", 46)
    laneclear_key = cfg.get_int("laneclear_key", 47)
    flee_key = cfg.get_int("flee_key", 30)

    use_q_in_combo = cfg.get_bool("use_q_in_combo", True)
    use_w_in_combo = cfg.get_bool("use_w_in_combo", True)
    use_e_in_combo = cfg.get_bool("use_e_in_combo", True)
    use_r_in_combo = cfg.get_bool("use_r_in_combo", True)

    draw_q_range = cfg.get_bool("draw_q_range", False)
    draw_w_range = cfg.get_bool("draw_w_range", False)
    draw_e_range = cfg.get_bool("draw_e_range", False)
    draw_r_range = cfg.get_bool("draw_r_range", False)

    steal_kill_with_w = cfg.get_bool("steal_kill_with_w", False)
    steal_kill_with_r = cfg.get_bool("steal_kill_with_r", False)


def LViewPlus64_save_cfg(cfg):
    global combo_key, harass_key, laneclear_key, killsteal_key, flee_key
    global use_q_in_combo, use_w_in_combo, use_e_in_combo, use_r_in_combo
    global steal_kill_with_w, steal_kill_with_r
    global draw_q_range, draw_w_range, draw_e_range, draw_r_range
    global laneclear_with_q

    cfg.set_int("combo_key", combo_key)
    cfg.set_int("harass_key", harass_key)
    cfg.set_int("killsteal_key", killsteal_key)
    cfg.set_int("laneclear_key", laneclear_key)
    cfg.set_int("flee_key", flee_key)

    cfg.set_bool("use_q_in_combo", use_q_in_combo)
    cfg.set_bool("use_w_in_combo", use_w_in_combo)
    cfg.set_bool("use_e_in_combo", use_e_in_combo)
    cfg.set_bool("use_r_in_combo", use_r_in_combo)

    cfg.set_bool("draw_q_range", draw_q_range)
    cfg.set_bool("draw_w_range", draw_w_range)
    cfg.set_bool("draw_e_range", draw_e_range)
    cfg.set_bool("draw_r_range", draw_r_range)

    cfg.set_bool("steal_kill_with_w", steal_kill_with_w)
    cfg.set_bool("steal_kill_with_r", steal_kill_with_r)


def LViewPlus64_draw_settings(game, ui):
    global combo_key, harass_key, laneclear_key, killsteal_key, flee_key
    global use_q_in_combo, use_w_in_combo, use_e_in_combo, use_r_in_combo
    global steal_kill_with_w, steal_kill_with_r
    global draw_q_range, draw_w_range, draw_e_range, draw_r_range
    global laneclear_with_q

    ui.begin("WS+ Jinx")
    combo_key = ui.keyselect("Combo key", combo_key)
    harass_key = ui.keyselect("Harass key", harass_key)
    killsteal_key = ui.keyselect("Killsteal key", killsteal_key)
    laneclear_key = ui.keyselect("Laneclear key", laneclear_key)
    flee_key = ui.keyselect("Flee key", flee_key)

    if ui.treenode("Setting [Q]"):
        use_q_in_combo = ui.checkbox("Use Q in Combo", use_q_in_combo)
        laneclear_with_q = ui.checkbox("Laneclear with q", laneclear_with_q)
        draw_q_range = ui.checkbox("Draw Q Range", draw_q_range)
        ui.treepop()
    if ui.treenode("Setting [W]"):
        use_w_in_combo = ui.checkbox("Use W in Combo", use_w_in_combo)
        steal_kill_with_w = ui.checkbox("Stealkill with w", steal_kill_with_w)
        draw_w_range = ui.checkbox("Draw W Range", draw_w_range)
        ui.treepop()
    if ui.treenode("Setting [E]"):
        use_e_in_combo = ui.checkbox("Use E in Combo", use_e_in_combo)
        draw_e_range = ui.checkbox("Draw E Range", draw_e_range)
        ui.treepop()
    if ui.treenode("Setting [R]"):
        use_r_in_combo = ui.checkbox("Use R in Combo", use_r_in_combo)
        steal_kill_with_r = ui.checkbox("Stealkill with r", steal_kill_with_r)
        draw_r_range = ui.checkbox("Draw R Range", draw_r_range)
        ui.treepop()

    ui.end()


qDamages = [20, 40, 55]
rDamages = [250, 350, 450]


def CalcRDmg(game, unit):
    global qDamages
    damage = 0
    distance = game.player.pos.distance(unit.pos)
    mathdist = math.floor(math.floor(distance) / 100)
    level = game.player.R.level
    baseq = rDamages[level - 1] + 0.15 + game.player.bonus_atk
    qmissheal = qDamages[level - 1] / 100 * (unit.max_health - unit.health)
    if distance < 100:
        damage = baseq + qmissheal
    elif distance >= 1500:
        damage = baseq + 10 + qmissheal
    else:
        damage = ((((mathdist * 6) + 10) / 100) * baseq) + baseq + qmissheal
    return rDamages[level - 1] + game.player.bonus_atk


def GetEnemyCount(game, dist):
    count = 0
    for champ in game.champs:
        if (
            champ
            and champ.is_visible
            and champ.is_enemy_to(game.player)
            and champ.isTargetable
            and champ.is_alive
            and game.is_point_on_screen(champ.pos)
            and game.distance(game.player, champ) <= dist
        ):
            count = count + 1
    return count


lastQ = 0
lastW = 0
lastE = 0
lastR = 0


def Swap(game, target):
    # if game.player.atkRange < 599:
    #     if game.player.pos.distance(target.pos) > 600 + target.gameplay_radius and game.player.pos.distance(target.pos) < (game.player.Q.level*25) + 75 + 600 + target.gameplay_radius:
    #         q_spell.trigger(False)
    # elif game.player.pos.distance(target.pos) < 600 + target.gameplay_radius and game.player.atkRange > 600:
    #     q_spell.trigger(False)
    if game.player.atkRange < 599:
        if (
            getBuff(game.player, "jinxqramp")
            and getBuff(game.player, "jinxqramp").countAlt == 3
        ) and game.player.pos.distance(target.pos) < (
            game.player.Q.level * 25
        ) + 75 + 600:
            q_spell.trigger(False)
        if (
            game.player.pos.distance(target.pos) > 600 + target.gameplay_radius
            and game.player.pos.distance(target.pos)
            < (game.player.Q.level * 25) + 75 + 600 + target.gameplay_radius
        ):
            q_spell.trigger(False)
        if GetEnemyCount(game, 600) > 1 and (
            getBuff(game.player, "jinxqramp")
            and getBuff(game.player, "jinxqramp").countAlt > 2
        ):
            q_spell.trigger(False)
    else:
        if (
            getBuff(game.player, "jinxqramp")
            and getBuff(game.player, "jinxqramp").countAlt < 3
        ) and game.player.pos.distance(target.pos) < 600 + target.gameplay_radius:
            q_spell.trigger(False)
        if game.player.pos.distance(target.pos) < 600 + target.gameplay_radius:
            q_spell.trigger(False)


def Combo(game):
    global lastQ, lastW, lastE, lastR
    q_spell = getSkill(game, "Q")
    w_spell = getSkill(game, "W")
    e_spell = getSkill(game, "E")
    r_spell = getSkill(game, "R")

    if (
        use_q_in_combo
        and IsReady(game, q_spell)
        and game.player.mana > 20
        and lastQ + 1 < game.time
    ):
        target = GetBestTargetsInRange(game, (game.player.Q.level * 25) + 75 + 600)
        if target:
            if game.player.atkRange < 599:
                if (
                    game.player.pos.distance(target.pos) > 600 + target.gameplay_radius
                    and game.player.pos.distance(target.pos)
                    < (game.player.Q.level * 25) + 75 + 600 + target.gameplay_radius
                ):
                    q_spell.trigger(False)
                    lastQ = game.time
            elif (
                game.player.pos.distance(target.pos) < 600 + target.gameplay_radius
                and game.player.atkRange > 600
            ):
                q_spell.trigger(False)
                lastQ = game.time
    if (
        use_w_in_combo
        and IsReady(game, w_spell)
        and game.player.mana > 90
        and lastW + 1 < game.time
    ):
        target = GetBestTargetsInRange(game, 1450)
        if target and not IsCollisioned(
            game, target
        ):  # and game.player.pos.distance(target.pos) > (game.player.Q.level*25) + 75 + 600:
            w_spell.move_and_trigger(game.world_to_screen(target.pos))
            lastW = game.time
    if (
        use_e_in_combo
        and IsReady(game, e_spell)
        and game.player.mana > 90
        and lastE + 1 < game.time
    ):
        target = GetBestTargetsInRange(game, 900)
        if target:
            e_spell.move_and_trigger(game.world_to_screen(target.pos))
            lastE = game.time
    if (
        use_r_in_combo
        and IsReady(game, r_spell)
        and game.player.mana > 100
        and lastR + 1 < game.time
    ):
        target = GetBestTargetsInRange(game, 12500)
        if target and CalcRDmg(game, target) >= target.health:
            r_spell.move_and_trigger(game.world_to_screen(target.pos))
            lastR = game.time


def Laneclear(game):
    global lastQ
    q_spell = getSkill(game, "Q")
    if (
        laneclear_with_q
        and IsReady(game, q_spell)
        and (game.player.mana / game.player.max_mana * 100) > 40
        and lastQ + 1 < game.time
    ):
        minion = GetBestMinionsInRange(game, (game.player.Q.level * 25) + 75 + 600)
        if minion:
            if game.player.atkRange < 599:
                if (
                    game.player.pos.distance(minion.pos) > 600 + minion.gameplay_radius
                    and game.player.pos.distance(minion.pos)
                    < (game.player.Q.level * 25) + 75 + 600 + minion.gameplay_radius
                ):
                    q_spell.trigger(False)
                    lastQ = game.time
            elif (
                game.player.pos.distance(minion.pos) < 600 + minion.gameplay_radius
                and game.player.atkRange > 600
            ):
                q_spell.trigger(False)
                lastQ = game.time


def LViewPlus64_update(game, ui):
    self = game.player

    if self.is_alive and game.is_point_on_screen(self.pos) and not game.isChatOpen:
        if game.is_key_down(combo_key):
            Combo(game)
        if game.is_key_down(laneclear_key):
            Laneclear(game)
