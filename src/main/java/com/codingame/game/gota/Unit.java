package com.codingame.game.gota;

public class Unit {
    // 0 white, 1 black
    public int owner;
    Cell cell;

    Unit(Cell cell, int owner) {
        this.cell = cell;
        this.owner = owner;
    }

    void moveTo(Cell cell) {
        this.cell.unit = null;
        this.cell = cell;
        this.cell.unit = this;
    }

    int getX() {
        return cell.x;
    }

    int getY() {
        return cell.y;
    }

    @Override
    public String toString() {
        return cell.toString();
    }
}
