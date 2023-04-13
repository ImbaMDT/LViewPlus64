from LViewPlus64 import *
from pprint import pprint

LViewPlus64_script_info = {
    "script": "Object Tracker",
    "author": "LView team + bckd00r",
    "description": "object tracker",
}


def draw_spell(spell, ui):
    if ui.treenode(str(spell.slot)):
        ui.labeltext("name", spell.name)
        ui.labeltext("missileName", spell.missileName)
        ui.labeltext("summmoner_spell_type", str(spell.summoner_spell_type))
        ui.dragint("level", spell.level)
        ui.dragfloat("ready_at", spell.ready_at)
        ui.dragfloat("value", spell.value)
        ui.dragfloat("timeCharge", spell.timeCharge)

        ui.separator()
        ui.dragfloat("speed", spell.speed)
        ui.dragfloat("cast_range", spell.cast_range)
        ui.dragfloat("width", spell.width)
        ui.dragfloat("cast_radius", spell.cast_radius)
        ui.dragfloat("height", spell.height)
        ui.dragfloat("delay", spell.delay)

        ui.treepop()


def draw_items(items, ui):
    for item in items:
        if ui.treenode(str(item.id)):
            ui.dragint("slot", item.slot)
            if item.movement_speed > 0:
                ui.dragfloat("movement_speed", item.movement_speed)
            if item.health > 0:
                ui.dragfloat("health", item.health)
            if item.crit > 0:
                ui.dragfloat("crit", item.crit)
            if item.ability_power > 0:
                ui.dragfloat("ability_power", item.ability_power)
            if item.mana > 0:
                ui.dragfloat("mana", item.mana)
            if item.armour > 0:
                ui.dragfloat("armour", item.armour)
            if item.magic_resist > 0:
                ui.dragfloat("magic_resist", item.magic_resist)
            if item.physical_damage > 0:
                ui.dragfloat("physical_damage", item.physical_damage)
            if item.attack_speed > 0:
                ui.dragfloat("attack_speed", item.attack_speed)
            if item.life_steal > 0:
                ui.dragfloat("life_steal", item.life_steal)
            if item.hp_regen > 0:
                ui.dragfloat("hp_regen", item.hp_regen)
            if item.movement_speed_percent > 0:
                ui.dragfloat("movement_speed_percent", item.movement_speed_percent)

            ui.treepop()


def draw_buffs(buffs, ui):
    for buff in buffs:
        if ui.treenode(str(buff.name)):
            ui.checkbox("isAlive", buff.isAlive)
            if buff.count > 0:
                ui.labeltext("count", str(buff.count))
            if buff.countAlt > 0:
                ui.labeltext("countAlt", str(buff.countAlt))
            if buff.type > 0:
                ui.dragint("type", buff.type)
            if buff.start_time > 0:
                ui.dragfloat("start_time", buff.start_time)
            if buff.end_time > 0:
                ui.dragfloat("end_time", buff.end_time)
            ui.treepop()


def draw_missile(obj, ui):
    if ui.treenode(
        "{}_({}->{}) ({})".format(obj.name, obj.src_id, obj.dest_id, hex(obj.address))
    ):
        ui.dragint("id", obj.id)
        ui.labeltext("net_id", hex(obj.net_id))
        ui.dragint("team", obj.team)

        ui.labeltext(
            "start_pos",
            f"x={obj.start_pos.x:.2f}, y={obj.start_pos.y:.2f}, z={obj.start_pos.z:.2f}",
        )
        ui.labeltext(
            "end_pos",
            f"x={obj.end_pos.x:.2f}, y={obj.end_pos.y:.2f}, z={obj.end_pos.z:.2f}",
        )
        ui.labeltext("pos", f"x={obj.pos.x:.2f}, y={obj.pos.y:.2f}, z={obj.pos.z:.2f}")
        ui.dragint("src_id", obj.src_id)
        ui.dragint("dest_id", obj.dest_id)

        ui.separator()
        ui.dragfloat("speed", obj.speed)
        ui.dragfloat("cast_range", obj.cast_range)
        ui.dragfloat("width", obj.width)
        ui.dragfloat("cast_radius", obj.cast_radius)
        ui.dragfloat("height", obj.height)
        ui.dragfloat("delay", obj.delay)
        ui.treepop()


def draw_game_object(obj, ui, additional_draw=None, set_open=False):
    if obj == None:
        ui.text("null", Color.RED)
        return
    if set_open:
        ui.opennext()
    if ui.treenode("{}_{} ({})".format(obj.name, obj.id, hex(obj.address))):
        ui.labeltext("address", hex(obj.address))
        ui.labeltext("net_id", hex(obj.net_id))
        ui.labeltext("name", obj.name, Color.ORANGE)
        ui.labeltext("pos", f"x={obj.pos.x:.2f}, y={obj.pos.y:.2f}, z={obj.pos.z:.2f}")
        ui.dragint("id", obj.id)

        ui.separator()
        ui.dragfloat("health", obj.health)
        ui.dragfloat("lvl", obj.lvl)
        ui.dragfloat("mana", obj.mana)
        ui.dragfloat("max_mana", obj.max_mana)
        ui.dragfloat("health_regen", obj.health_regen)
        ui.dragfloat("mana_regen", obj.mana_regen)
        ui.checkbox("is_alive", obj.is_alive)

        ui.separator()
        ui.dragfloat("currentDashSpeed", obj.currentDashSpeed)
        ui.checkbox("isDashing", obj.isDashing)
        ui.checkbox("dIsMoving", obj.dIsMoving)
        ui.labeltext(
            "dashPos",
            f"x={obj.dashPos.x:.2f}, y={obj.dashPos.y:.2f}, z={obj.dashPos.z:.2f}",
        )

        ui.separator()
        ui.dragfloat("base_atk", obj.base_atk)
        ui.dragfloat("bonus_atk", obj.bonus_atk)
        ui.dragfloat("armour", obj.armour)
        ui.dragfloat("magic_resist", obj.magic_resist)
        ui.dragfloat("ap", obj.ap)
        ui.dragfloat("crit", obj.crit)
        ui.dragfloat("crit_multi", obj.crit_multi)

        ui.separator()
        ui.dragfloat("atk_range", obj.atk_range)
        ui.dragfloat("base_atk_range", obj.base_atk_range)
        ui.dragfloat("base_atk_speed", obj.base_atk_speed)
        ui.dragfloat("atk_speed_multi", obj.atk_speed_multi)
        ui.dragfloat("atk_speed_ratio", obj.atk_speed_ratio)
        ui.dragfloat("basic_missile_speed", obj.basic_missile_speed)
        ui.dragfloat("base_ms", obj.base_ms)
        ui.dragfloat("movement_speed", obj.movement_speed)

        ui.separator()
        ui.dragfloat("selection_radius", obj.selection_radius)
        ui.dragfloat("gameplay_radius", obj.gameplay_radius)
        ui.dragfloat("pathing_radius", obj.pathing_radius)
        ui.dragfloat("acquisition_radius", obj.acquisition_radius)

        ui.separator()
        ui.dragfloat("duration", obj.duration)
        ui.dragfloat("last_visible_at", obj.last_visible_at)

        ui.text("Buffs")
        draw_buffs(obj.buffs, ui)

        ui.checkbox("is_visible", obj.is_visible)

        if additional_draw != None:
            additional_draw()

        ui.treepop()


def draw_champion(obj, ui):
    def draw_spells():
        ui.dragint("Level", obj.lvl)

        ui.text("Items")
        draw_items(obj.items, ui)

        ui.text("Buffs")
        draw_buffs(obj.buffs, ui)

        ui.text("Skills")
        draw_spell(obj.Q, ui)
        draw_spell(obj.W, ui)
        draw_spell(obj.E, ui)
        draw_spell(obj.R, ui)
        draw_spell(obj.D, ui)
        draw_spell(obj.F, ui)

    draw_game_object(obj, ui, additional_draw=draw_spells)


def draw_list(label, objs, ui, draw_func):
    if ui.treenode(label):
        for obj in objs:
            draw_func(obj, ui)
        ui.treepop()


def LViewPlus64_load_cfg(cfg):
    pass


def LViewPlus64_save_cfg(cfg):
    pass


def LViewPlus64_draw_settings(objs, ui):
    pass


def LViewPlus64_update(game, ui):
    ui.begin("Object Viewer")

    ui.dragfloat("time", game.time)
    ui.dragint("chat", game.isChatOpen)
    ui.dragint("ping", game.ping)
#    ui.dragfloat("mousePos", game.mousePos)
    ui.labeltext("mousePos", f"x={game.mousePos.x:.2f}, y={game.mousePos.y:.2f}, z={game.mousePos.z:.2f}")

    if game.hovered_obj:
        ui.labeltext(
            "hovered_obj", f"{game.hovered_obj.name} ({hex(game.hovered_obj.address)})"
        )
    else:
        ui.labeltext("hovered_obj", "none")

    ui.text("Local Champion")
    draw_champion(game.player, ui)

    ui.text("Lists")
    draw_list("Champions", game.champs, ui, draw_champion)
    draw_list("Minions", game.minions, ui, draw_game_object)
    draw_list("Jungle", game.jungle, ui, draw_game_object)
    draw_list("Turrets", game.turrets, ui, draw_game_object)
    draw_list("Missiles", game.missiles, ui, draw_missile)
    draw_list("Others", game.others, ui, draw_game_object)

    ui.end()


    for obj in game.minions:
        if game.is_point_on_screen(obj.pos):
            navBegin = game.world_to_screen(obj.navBegin)
            navEnd = game.world_to_screen(obj.navEnd)
            game.draw_line(Vec2(navBegin.x, navBegin.y), Vec2(navEnd.x, navEnd.y), 2, Color.YELLOW)
