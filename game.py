import pygame
import sys


pygame.init()


image_width=512
image_height=512

total_width=512+112
total_height=512
screen = pygame.display.set_mode((total_height,total_width))
pygame.display.set_caption("My First Game Window")


running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:  
            running = False

  
    screen.fill((0,0,0),(0,0,image_height,image_width))

    
    pygame.display.flip()


pygame.quit()
sys.exit()
