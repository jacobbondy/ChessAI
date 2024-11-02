# -*- coding: utf-8 -*-
"""
Created on Sat Oct  5 21:47:24 2024

If you need to recreate the environment for some reason
Uninstall python in environment then run:
conda install pytorch::pytorch=*=*cuda* spyder

python version: 3.10.15
pytorch version: 1.12.1 Build py3.10_cuda11.3_cudnn8_0

@author: Jacob Bondy
"""

import torch, tkinter as tk
from PIL import ImageTk, Image
import bitboard
import time as t;

chessBoard = bitboard.Board()

Board = chessBoard.getBoard()

start = t.time()
test = [chessBoard.getBishopMove(i) for i in range(64)]
print("Total time: ", t.time() - start)



class ChessBoard(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
        self.rows = 8
        self.columns = 8
        self.cell_width = 60
        self.cell_height = 60
        
        self.canvas = tk.Canvas(self, width=self.columns*self.cell_width, height=self.rows*self.cell_height, borderwidth=0, highlightthickness=0)
        self.canvas.pack(side="top", fill="both", expand="true")
        
        self.board = {}
        self.original_colors = {}
        self.white_piece_images = {
            'K': ImageTk.PhotoImage(Image.open("./Pieces/White/King.png")),
            'Q': ImageTk.PhotoImage(Image.open("./Pieces/White/Queen.png")),
            'R': ImageTk.PhotoImage(Image.open("./Pieces/White/Rook.png")),
            'B': ImageTk.PhotoImage(Image.open("./Pieces/White/Bishop.png")),
            'N': ImageTk.PhotoImage(Image.open("./Pieces/White/Knight.png")),
            'P': ImageTk.PhotoImage(Image.open("./Pieces/White/Pawn.png"))
        }
        
        for column in range(self.columns):
            for row in range(self.rows):
                x1 = column * self.cell_width
                y1 = row * self.cell_height
                x2 = x1 + self.cell_width
                y2 = y1 + self.cell_height
                color = "#779952" if (row + column) % 2 == 0 else "#edeed1"
                self.board[row, column] = self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, tags="rect")
                self.original_colors[row, column] = color  # Store original color

        self.canvas.bind("<Button-1>", self.on_square_click)
        self.canvas.bind("<Button-2>", self.clear)
        
        self.canvas.create_image(30, 30, image=self.white_piece_images['K'])

    def on_square_click(self, event):
        # Get the coordinates of the click
        x, y = event.x, event.y
        
        # Find the row and column based on the click
        column = x // self.cell_width
        row = y // self.cell_height
        
        print(f"Column: {column} Row: {row}, Position: {row*8 + column}")
        
        if (row, column) in self.board:
            self.drawMoves(column, row)

    def drawMoves(self, column, row):
        position = (column + row*8)
        
        moves = chessBoard.getBishopMove(position)
        # moves = test[position]
        
        for i in range(8):
            for j in range(8):
                if moves[i][j] == 1:
                    rect = self.board[i,j]
                    self.canvas.itemconfig(rect, fill="#ffcccc")
                    
    def clear(self, event):
        for i in range(8):
            for j in range(8):
                rect = self.board[i,j]
                self.canvas.itemconfig(rect, fill=self.original_colors[i,j])
                

if __name__ == "__main__":
    chessboard = ChessBoard("Chess")
    chessboard.mainloop()