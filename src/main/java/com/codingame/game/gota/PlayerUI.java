package com.codingame.game.gota;

import com.codingame.game.Player;
import com.codingame.gameengine.module.entities.Rectangle;
import com.codingame.gameengine.module.entities.Sprite;
import com.codingame.gameengine.module.entities.Text;

public class PlayerUI {
    Player player;
    Rectangle rectangle;
    Sprite avatar;
    Sprite piece;
    public Text action;
    public Text msg;

    PlayerUI(Viewer viewer, Player player) {
        int START_Y = 200;
        int START_X = player.getIndex() == 1 ? viewer.graphics.getWorld().getWidth() - 350 : 50;
        this.player = player;

        rectangle = viewer.graphics.createRectangle().setHeight(600).setWidth(300).setX(START_X).setY(START_Y).setFillColor(0xFFFFFF).setAlpha(0.15);

        viewer.graphics.createText(player.getNicknameToken()).setFontSize(42).setX(START_X + 150).setAnchorX(0.5).setY(START_Y + 10).setFillColor(0xffffff);
        avatar = viewer.graphics.createSprite().setImage(player.getAvatarToken()).setX(START_X + 150).setY(START_Y + 120).setAnchorX(0.5).setBaseHeight(100).setBaseWidth(100);
        piece = viewer.graphics.createSprite().setImage(player.getIndex() == 0 ? "w.png" : "b.png").setX(START_X + 150 - viewer.CIRCLE_RADIUS).setY(START_Y + 270).setBaseWidth(viewer.CIRCLE_RADIUS * 2).setBaseHeight(viewer.CIRCLE_RADIUS * 2);
        action = viewer.graphics.createText().setFillColor(0xffffff).setAnchorX(0.5).setX(START_X + 150).setY(START_Y + 390).setFontSize(40);
        msg = viewer.graphics.createText().setFillColor(0xffffff).setAnchorX(0.5).setX(START_X + 150).setY(START_Y + 460).setFontSize(36);
    }
}
