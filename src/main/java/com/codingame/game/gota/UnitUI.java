package com.codingame.game.gota;

import com.codingame.gameengine.module.entities.Sprite;

public class UnitUI {
    Unit unit;
    Sprite sprite;

    UnitUI(Unit unit, Viewer viewer) {
        int x = unit.getX();
        int y = unit.getY();

        this.unit = unit;
        sprite = viewer.graphics.createSprite()
                .setImage(unit.owner == 0 ? "w.png" : "b.png")
                .setX(100)
                .setY(200)
                .setBaseWidth(viewer.CIRCLE_RADIUS * 2)
                .setBaseHeight(viewer.CIRCLE_RADIUS * 2)
                .setX(viewer.rectangles[y][x].getX() + viewer.GAP)
                .setY(viewer.rectangles[y][x].getY() + viewer.GAP);
    }
}
