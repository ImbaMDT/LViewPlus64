#pragma once
#include <boost/python.hpp>
#include "MemSnapshot.h"
#include "Utils.h"

using namespace boost::python;

/// Interface used by python scripts for game related stuff
class PyGame {

public:
	std::map<int, float>  distanceCache;
	MemSnapshot*          ms;
	ImDrawList*           overlay;

public:
	PyGame() {}

	// Exposed Fields
	boost::python::list                  champs, minions, turrets, jungle, missiles, others;
	float                 gameTime;
	bool				  isChatOpen;
	int					  ping;
	Vector3				  mousePos;
					      
	MapObject*            map;
	GameObject*           hoveredObject;
	GameObject*           localChampion;

	object GetHoveredObject() { 
		if (hoveredObject == nullptr)
			return object();
		return object(boost::ref(*hoveredObject)); 
	}

	object GetLocalChampion() { 
		if (localChampion == nullptr)
			return object();
		return object(boost::ref(*localChampion)); 
	};

	object GetMap() {
		return object(boost::ref(*map));
	}

	//Exposed methods
	Vector2 WorldToScreen(const Vector3& pos) {
		return ms->renderer->WorldToScreen(pos);
	}

	Vector2 WorldToMinimap(const Vector3& pos) {
		return ms->renderer->WorldToMinimap(pos, ms->minimapPos, ms->minimapSize);
	}

	float DistanceToMinimap(float dist) {
		return ms->renderer->DistanceToMinimap(dist, ms->minimapSize);
	}

	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(IsScreenPointOnScreenOverloads, IsScreenPointOnScreen, 1, 3);
	bool IsScreenPointOnScreen(const Vector2& point, float offsetX = 0.f, float offsetY = 0.f) {
		return ms->renderer->IsScreenPointOnScreen(point, offsetX, offsetY);
	}

	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(IsWorldPointOnScreenOverloads, IsWorldPointOnScreen, 1, 3);
	bool IsWorldPointOnScreen(const Vector3& point, float offsetX = 0.f, float offsetY = 0.f) {
		return ms->renderer->IsWorldPointOnScreen(point, offsetX, offsetY);
	}

	void DrawCircle(const Vector2& center, float radius, int numPoints, float thickness, const ImVec4& color) {
		overlay->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color), numPoints, thickness);
	}

	void DrawCircleFilled(const Vector2& center, float radius, int numPoints, const ImVec4& color) {
		overlay->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color), numPoints);
	}

	void DrawTxt(const Vector2& pos, const char* text, const ImVec4& color) {
		overlay->AddText(ImVec2(pos.x, pos.y), ImColor(color), text);
	}

	void MoveCursor(const Vector2& pos) {
		Input::Move(pos.x, pos.y);
	}

	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(DrawRectOverloads, DrawRect, 2, 4);
	void DrawRect(const Vector4& box, const ImVec4& color, float rounding = 0, float thickness = 1.0) {
		overlay->AddRect(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding, 15, thickness);
	}

	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(DrawRectFilledOverloads, DrawRectFilled, 2, 3);
	void DrawRectFilled(const Vector4& box, const ImVec4& color, float rounding = 0) {
		overlay->AddRectFilled(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding);
	}

	void DrawRectWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, float thickness, const ImVec4& color) {
		static Vector2 points[4];
		points[0] = ms->renderer->WorldToScreen(p1);
		points[1] = ms->renderer->WorldToScreen(p2);
		points[2] = ms->renderer->WorldToScreen(p3);
		points[3] = ms->renderer->WorldToScreen(p4);

		overlay->AddPolyline((ImVec2*)points, 4, ImColor(color), true, thickness);
	}

	void DrawTriangleWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, float thickness, const ImVec4& color) {
		overlay->AddTriangle(
			(ImVec2&)ms->renderer->WorldToScreen(p1), 
			(ImVec2&)ms->renderer->WorldToScreen(p2),
			(ImVec2&)ms->renderer->WorldToScreen(p3), ImColor(color), thickness);
	}

	void DrawTriangleWorldFilled(const Vector3& p1, const Vector3& p2, const Vector3& p3, const ImVec4& color) {
		overlay->AddTriangleFilled(
			(ImVec2&)ms->renderer->WorldToScreen(p1), 
			(ImVec2&)ms->renderer->WorldToScreen(p2),
			(ImVec2&)ms->renderer->WorldToScreen(p3), ImColor(color));
	}

	void DrawCircleWorld(const Vector3& center, float radius, int numPoints, float thickness, const ImVec4& color) {
		ms->renderer->DrawCircleAt(overlay, center, radius, false, numPoints, ImColor(color), thickness);
	}

	void DrawCircleWorldFilled(const Vector3& center, float radius, int numPoints, const ImVec4& color) {
		ms->renderer->DrawCircleAt(overlay, center, radius, true, numPoints, ImColor(color));
	}

	void DrawLine(const Vector2& start, const Vector2& end, float thickness, const ImVec4& color) {
		overlay->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
	}

	void DrawImage(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color) {
		static ImVec2 zero = ImVec2(0.f, 0.f);
		static ImVec2 one = ImVec2(1.f, 1.f);

		auto it = GameData::Images.find(std::string(img));
		if (it == GameData::Images.end())
			return;
		overlay->AddImage(it->second->resourceView, (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color));
	}

	void DrawImageRounded(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color, float rounding) {
		static ImVec2 zero = ImVec2(0.f, 0.f);
		static ImVec2 one = ImVec2(1.f, 1.f);

		auto it = GameData::Images.find(std::string(img));
		if (it == GameData::Images.end())
			return;
		overlay->AddImageRounded(it->second->resourceView, (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color), rounding);
		
	}

	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(DrawButtonOverloads, DrawButton, 4, 5);
	void DrawButton(const Vector2& p, const char* text, ImVec4& colorButton, ImVec4& colorText, float rounding = 0) {
		int txtSize = strlen(text);
		overlay->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + txtSize * 7.2f + 5, p.y + 17), ImColor(colorButton), rounding);
		overlay->AddText(ImVec2(p.x + 5, p.y + 2), ImColor(colorText), text);
	}

	Vector2 HpBarPos(GameObject& obj) {
		Vector3 pos = obj.position.clone();
		pos.y += obj.GetHpBarHeight();

		Vector2 w2s = ms->renderer->WorldToScreen(pos);
		w2s.y -= (ms->renderer->height * 0.00083333335f * obj.GetHpBarHeight());

		return w2s;
	}

	Vector3 clamp_norm_2d(const Vector3& v, float n_max) {
		float vx = v.x, vy = v.y, vz = v.z;
		float n = sqrt(pow(vx, 2.f) + pow(vz, 2.f));
		float f = min(n, n_max) / n;

		return Vector3(f * vx, vy, f * vz);
	}

	bool isLeft(const Vector2& a, const Vector2& b, const Vector2& c) {
		return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
	}

	bool PointOnLineSegment(const Vector2& pt1, const Vector2& pt2, const Vector2& pt, double epsilon = 0.001)
	{
		if (pt.x - max(pt1.x, pt2.x) > epsilon ||
			min(pt1.x, pt2.x) - pt.x > epsilon ||
			pt.y - max(pt1.y, pt2.y) > epsilon ||
			min(pt1.y, pt2.y) - pt.y > epsilon)
			return false;

		if (abs(pt2.x - pt1.x) < epsilon)
			return abs(pt1.x - pt.x) < epsilon || abs(pt2.x - pt.x) < epsilon;
		if (abs(pt2.y - pt1.y) < epsilon)
			return abs(pt1.y - pt.y) < epsilon || abs(pt2.y - pt.y) < epsilon;

		double x = pt1.x + (pt.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
		double y = pt1.y + (pt.x - pt1.x) * (pt2.y - pt1.y) / (pt2.x - pt1.x);

		return abs(pt.x - x) < epsilon || abs(pt.y - y) < epsilon;
	}

	bool isPointOnLineSegment(const Vector2& point, const Vector2& start, const Vector2& end)
	{
		if (max(start.x, end.x) > point.x && point.x > min(start.x, end.x)
			&& max(start.y, end.y) > point.y && point.y > min(start.y, end.y))
		{
			return true;
		}

		return false;
	}

	int compareTo(int a, int b)
	{
		if (a < b) return -1;
		else if (a == b) return 0;
		else return 1;
	}

	void PressKey(int key) {
		return Input::PressKey((HKey)key);
	}

	bool WasKeyPressed(int key) {
		return Input::WasKeyPressed((HKey)key);
	}

	void PressLeftClick() {
		return Input::PressLeftClick();
	}

	void PressRightClick() {
		return Input::PressRightClick();
	}

	void MouseRightDown() {
		return Input::MouseRightDown();
	}

	void MouseRightUp() {
		return Input::MouseRightUp();
	}

	void MouseLeftDown() {
		return Input::MouseLeftDown();
	}

	void MouseLeftUp() {
		return Input::MouseLeftUp();
	}

	void ClickAt(bool leftClick, const Vector2& pos) {
		return Input::ClickAt(leftClick, pos.x, pos.y);
	}

	bool IsKeyDown(int key) {
		return Input::IsKeyDown((HKey)key);
	}

	Vector2 GetCursor() {
		return Input::GetCursorPosition();
	}

	SpellInfo* GetSpellInfo(const char* spellName) {
		std::string name(spellName);
		return GameData::GetSpellInfoByName(name);
	}

	float Distance(GameObject* first, GameObject* second) {
		
		int key = (first->objectIndex > second->objectIndex) ?
			(first->objectIndex << 16) | second->objectIndex : 
			(second->objectIndex << 16) | first->objectIndex;

		auto it = distanceCache.find(key);
		if (it != distanceCache.end())
			return it->second;

		float dist = first->position.distance(second->position);
		distanceCache[key] = dist;

		return dist;
	}

	GameObject* GetBestTarget(const UnitTag& unittype, float range) {
		GameObject* champion = nullptr;
		float lastHealth = FLT_MAX;
		if (!range || range == 0)
			range = localChampion->atkRange + localChampion->GetGameplayRadius();
		for (auto& champ : (unittype == Unit_Champion ? ms->champions : unittype == Unit_Minion_Lane ? ms->minions : unittype == Unit_Monster ? ms->jungle : unittype == Unit_Structure_Turret ? ms->turrets : ms->champions)) {
			if (champ->HasUnitTags(Unit_Champion_Clone))
				continue;
			if (champ->isAlive && champ->isVisible && champ->isTargetable && champ->IsEnemyTo(*localChampion) && lastHealth >= champ->health && Distance(champ.get(), localChampion) <= range + (champ->GetGameplayRadius() / 2)) {
				lastHealth = champ->health;
				champion = champ.get();
			}
		};
		return champion;
	}

	GameObject* GetObjectByIndex(short index) {
		auto it = ms->indexToNetId.find(index);
		if (it == ms->indexToNetId.end())
			return nullptr;

		auto it2 = ms->objectMap.find(it->second);
		if (it2 == ms->objectMap.end())
			return nullptr;

		return it2->second.get();
	}

	GameObject* GetObjectByNetId(int net_id) {
		auto it = ms->objectMap.find(net_id);
		return (it != ms->objectMap.end()) ? it->second.get() : nullptr;
	}

	static PyGame ConstructFromMemSnapshot(MemSnapshot& snapshot) {
		PyGame gs;

		gs.ms = &snapshot;
		gs.gameTime = snapshot.gameTime;
		gs.isChatOpen = snapshot.isChatOpen;
		gs.ping = snapshot.ping;
		gs.mousePos = snapshot.mousePos;
		gs.hoveredObject = snapshot.hoveredObject.get();
		gs.localChampion = snapshot.player.get();
		gs.map = snapshot.map.get();

		for (auto it = snapshot.champions.begin(); it != snapshot.champions.end(); ++it) {
			gs.champs.append(boost::ref(**it));
		}
		for (auto it = snapshot.minions.begin(); it != snapshot.minions.end(); ++it) {
			gs.minions.append(boost::ref(**it));
		}
		for (auto it = snapshot.turrets.begin(); it != snapshot.turrets.end(); ++it) {
			gs.turrets.append(boost::ref(**it));
		}
		for (auto it = snapshot.jungle.begin(); it != snapshot.jungle.end(); ++it) {
			gs.jungle.append(boost::ref(**it));
		}
		for (auto it = snapshot.missiles.begin(); it != snapshot.missiles.end(); ++it) {
			gs.missiles.append(boost::ref(**it));
		}
		for (auto it = snapshot.others.begin(); it != snapshot.others.end(); ++it) {
			gs.others.append(boost::ref(**it));
		}
		return gs;
	}
};