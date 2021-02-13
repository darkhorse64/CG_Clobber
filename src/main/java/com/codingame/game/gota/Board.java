package com.codingame.game.gota;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Board {
    long seed;
    int HEIGHT;
    int WIDTH;
    public Cell[][] cells;
    List<Unit> units;

    public int getHEIGHT() {
        return HEIGHT;
    }

    public int getWIDTH() {
        return WIDTH;
    }

    public Board(int size, long seed) {
        this.seed = seed;
        HEIGHT= size;
        WIDTH = size;
        cells = new Cell[HEIGHT][WIDTH];
        units = new ArrayList<>();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0;  x < WIDTH; ++x) {
                cells[y][x] = new Cell(x, y);
                units.add(cells[y][x].setUnit((x+y+1)&1));
            }
        }
    }

    // Returns if player lost.
    public boolean hasPlayerLost(int player) {
        for(Unit unit : units) {
            if (unit.owner != player) continue;
            for (Direction dir : Direction.values()) {
                int x = unit.getX() + dir.x;
                int y = unit.getY() + dir.y;
                if (!isValid(x, y, player)) continue;

                return false;
            }
        }
        return true;
    }

    public ArrayList<Action> getLegalActions(int player) {
        ArrayList<Action> actions = new ArrayList<>();

        for(Unit unit : units) {
            if(unit.owner != player) continue;
            Cell cell = unit.cell;

            for (Direction dir : Direction.values()) {
                int x = unit.getX();
                int y = unit.getY();

                x += dir.x;
                y += dir.y;

                if (!isValid(x, y, player)) continue;

                Action action = new Action(unit, cells[y][x]);

                actions.add(action);
            }
        }

        return actions;
    }

    public void applyAction(Unit unit, Cell target) {
        units.remove(target.unit);
        unit.moveTo(target);
    }

    boolean isInside(int x, int y) {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
    }

    boolean isValid(int x, int y, int owner) {
        if (!isInside(x, y)) return false;
        if (cells[y][x].unit == null || cells[y][x].unit.owner == owner) return false;
        return true;
    }
}
