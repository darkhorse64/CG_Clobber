package com.codingame.game.gota;

public class Action {
    public Unit unit;
    public Cell target;

    Action(Unit unit, Cell target) {
        this.unit = unit;
        this.target = target;
    }

    @Override
    public String toString() {
        return unit.toString() + target.toString();
    }
}
