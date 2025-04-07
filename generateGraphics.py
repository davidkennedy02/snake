from PIL import Image, ImageDraw
import random
import math
import os

SIZE = 30

def create_head_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    head_color = (50, 220, 20)  # vibrant lime green instead of brown
    
    # Draw rectangular head with the tongue side not extending to edge
    rect_x1, rect_y1 = 0, 0  # Left and top all the way to edge
    rect_x2, rect_y2 = SIZE, 26  # Right all the way to edge, bottom stops short for tongue
    
    # Main rectangle - fills most of the square but leaves room for tongue
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=head_color)
    
    # Eyes (for the base "up" orientation: eyes toward the bottom closer to tongue)
    eye_color = (255, 255, 255)
    pupil_color = (0, 0, 0)
    # Making eyes much bigger
    draw.ellipse([(6,15),(14,23)], fill=eye_color)  # Left eye - bigger
    draw.ellipse([(16,15),(24,23)], fill=eye_color)  # Right eye - bigger
    # Making pupils bigger too
    draw.ellipse([(8,17),(12,21)], fill=pupil_color)  # Left pupil - bigger
    draw.ellipse([(18,17),(22,21)], fill=pupil_color)  # Right pupil - bigger
    
    # Tongue (for head_up, tongue at bottom)
    tongue_color = (255, 0, 0)
    draw.line([(15,26), (15,30)], fill=tongue_color, width=2)
    
    # Define eye regions with safety margin
    eye_regions = [
        (5, 14, 15, 24),  # left eye with expanded margin
        (15, 14, 25, 24)   # right eye with expanded margin
    ]
    
    # Add snake scale texture
    # Create scales in a grid pattern that fits the rectangular head
    for y in range(4, SIZE-4, 3):
        offset = 1 if (y // 3) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(3 + offset, SIZE-3, 3):
            # Check if scale doesn't overlap with eyes
            if not any(ex1 <= x <= ex2 and ey1 <= y <= ey2 for ex1, ey1, ex2, ey2 in eye_regions):
                # Draw scale
                scale_width = 2
                scale_height = 1
                draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                             (x+scale_width/2, y+scale_height/2)], 
                             fill=(30, 180, 10))  # slightly darker green for scales
    return im

def create_body_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    body_color = (50, 220, 20)  # vibrant lime green instead of brown
    
    # Draw rectangular body extending to full edges
    # No rounded corners as body connects on both ends
    rect_x1, rect_y1 = 0, 0  # Left edge
    rect_x2, rect_y2 = SIZE, SIZE  # Right edge
    
    # Rectangle fills the entire area
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=body_color)
    
    # Scale dimensions
    scale_width = 2
    scale_height = 1
    
    # Create snake scale pattern on body
    for y in range(4, SIZE-4, 3):
        offset = 1 if (y // 3) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(3 + offset, SIZE-3, 3):
            # Draw scale
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(30, 180, 10))  # slightly darker green for scales
    return im

def create_tail_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    tail_color = (50, 220, 20)  # vibrant lime green instead of brown
    
    # Create a tapered rectangle for the tail that extends to edges
    # The top connects to the body and goes all the way to the edge
    rect_top = 0  # Top edge
    rect_bottom = 20  # Where tapering begins
    rect_left = 0  # Left edge
    rect_right = SIZE  # Right edge
    tip_y = SIZE  # Tip goes to bottom edge
    
    # Main rectangular part at the top - full width
    draw.rectangle([(rect_left, rect_top), (rect_right, rect_bottom)], fill=tail_color)
    
    # Draw the tapered end (triangle)
    draw.polygon([(rect_left, rect_bottom), (rect_right, rect_bottom), (SIZE//2, tip_y)], fill=tail_color)
    
    # Scale dimensions
    scale_width = 2
    scale_height = 1
    
    # Create snake scale pattern in the rectangular section
    for y in range(4, rect_bottom-2, 3):
        offset = 1 if (y // 3) % 2 == 0 else 0
        for x in range(3 + offset, SIZE-3, 3):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                          (x+scale_width/2, y+scale_height/2)], 
                          fill=(30, 180, 10))  # slightly darker green for scales
    
    # Add a few scales in the tapered section
    for y in range(rect_bottom + 2, tip_y - 2, 3):
        # Calculate width at this y level
        ratio = (y - rect_bottom) / (tip_y - rect_bottom)
        current_width = (1 - ratio) * SIZE
        current_left = SIZE//2 - current_width/2
        current_right = SIZE//2 + current_width/2
        
        offset = 1 if (y // 3) % 2 == 0 else 0
        for x in range(int(current_left) + offset + 2, int(current_right) - 2, 3):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(30, 180, 10))  # slightly darker green for scales
    
    return im

def create_apple():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    
    # Apple base color (bright red)
    apple_color = (220, 30, 30)
    
    # Draw the main apple body as a circle
    apple_radius = SIZE // 2 - 3
    apple_center = (SIZE // 2, SIZE // 2 + 2)  # Slightly lower to leave room for stem
    draw.ellipse([(apple_center[0] - apple_radius, apple_center[1] - apple_radius),
                  (apple_center[0] + apple_radius, apple_center[1] + apple_radius)], 
                 fill=apple_color)
    
    # Add a simple white diamond highlight instead of a gradient
    highlight_size = apple_radius // 7  # Smaller highlight
    highlight_center = (apple_center[0] - highlight_size, apple_center[1] - highlight_size)
    
    # Draw a simple white diamond shape
    diamond_points = [
        (highlight_center[0], highlight_center[1] - highlight_size),  # Top
        (highlight_center[0] + highlight_size, highlight_center[1]),  # Right
        (highlight_center[0], highlight_center[1] + highlight_size),  # Bottom
        (highlight_center[0] - highlight_size, highlight_center[1])   # Left
    ]
    draw.polygon(diamond_points, fill=(255, 255, 255, 100))  # Very light white with low alpha
    
    # Draw the stem
    stem_color = (139, 69, 19)  # Brown color for stem
    stem_width = 2
    stem_start = (SIZE // 2, apple_center[1] - apple_radius)
    stem_end = (SIZE // 2, stem_start[1] - 5)
    draw.line([stem_start, stem_end], fill=stem_color, width=stem_width)
    
    # Draw a small leaf
    leaf_color = (30, 180, 30)  # Green color for leaf
    leaf_start = (SIZE // 2, stem_end[1] + 2)
    leaf_points = [
        leaf_start,
        (leaf_start[0] + 6, leaf_start[1] - 3),
        (leaf_start[0] + 3, leaf_start[1] - 5),
        leaf_start
    ]
    draw.polygon(leaf_points, fill=leaf_color)
    
    return im

def create_background():
    # Create a 1000x1000 background
    bg_size = 1000
    fence_size = 900  # The fence boundary size
    
    bg = Image.new("RGBA", (bg_size, bg_size), (0,0,0,0))
    draw = ImageDraw.Draw(bg)
    
    # Base soil/dirt color for the field
    soil_color = (110, 80, 55)  # Lighter, warmer soil tone
    draw.rectangle([(0, 0), (bg_size, bg_size)], fill=soil_color)
    
    # Add soil texture variation to the field
    for i in range(2000):
        x = random.randint(0, bg_size-1)
        y = random.randint(0, bg_size-1)
        size = random.randint(3, 8)
        shade = random.randint(-15, 15)
        texture_color = (
            max(min(soil_color[0] + shade, 255), 0),
            max(min(soil_color[1] + shade, 255), 0),
            max(min(soil_color[2] + shade, 255), 0)
        )
        draw.ellipse([(x, y), (x+size, y+size)], fill=texture_color)
    
    # Add grass tufts for bird's-eye view
    grass_colors = [
        (60, 145, 60),   # Bright green
        (70, 160, 70),   # Light green
        (50, 130, 50),   # Medium green
        (40, 120, 50),   # Dark green
        (90, 170, 60),   # Yellow-green
        (100, 180, 70),  # Fresh green
    ]
    
    # Draw grass patches/tufts viewed from above
    for i in range(8000):
        x = random.randint(0, bg_size-1)
        y = random.randint(0, bg_size-1)
        
        # Random tuft size
        size_x = random.randint(2, 6)
        size_y = random.randint(2, 6)
        
        # Random angle for variety
        angle = random.randint(0, 360)
        color = random.choice(grass_colors)
        
        # Add slight color variation within each tuft
        color_var = random.randint(-10, 10)
        tuft_color = (
            max(min(color[0] + color_var, 255), 0),
            max(min(color[1] + color_var, 255), 0),
            max(min(color[2] + color_var, 255), 0)
        )
        
        # Create a rotated ellipse for the grass tuft
        points = []
        num_points = 12
        for j in range(num_points):
            t = j * (2 * math.pi / num_points)
            pre_rotate_x = size_x * math.cos(t)
            pre_rotate_y = size_y * math.sin(t)
            angle_rad = math.radians(angle)
            rotated_x = pre_rotate_x * math.cos(angle_rad) - pre_rotate_y * math.sin(angle_rad)
            rotated_y = pre_rotate_x * math.sin(angle_rad) + pre_rotate_y * math.cos(angle_rad)
            px = x + rotated_x
            py = y + rotated_y
            points.append((px, py))
        draw.polygon(points, fill=tuft_color)
        
        if random.random() > 0.7:
            detail_color = (
                max(min(tuft_color[0] - 15, 255), 0),
                max(min(tuft_color[1] - 5, 255), 0),
                max(min(tuft_color[2] - 15, 255), 0)
            )
            for _ in range(1, 3):
                detail_angle = random.randint(0, 360)
                detail_length = min(size_x, size_y) * 0.7
                detail_end_x = x + detail_length * math.cos(math.radians(detail_angle))
                detail_end_y = y + detail_length * math.sin(math.radians(detail_angle))
                draw.line([(x, y), (detail_end_x, detail_end_y)], fill=detail_color, width=1)
    
    # Add wildflowers viewed from above (circles with petals radiating outward)
    flower_colors = [
        (255, 255, 200),  # Pale yellow
        (255, 200, 200),  # Pale pink
        (200, 200, 255),  # Pale blue
        (255, 255, 255),  # White
        (255, 220, 180)   # Pale orange
    ]
    
    for i in range(120):
        x = random.randint(0, bg_size-1)
        y = random.randint(0, bg_size-1)
        
        size = random.randint(3, 7)
        color = random.choice(flower_colors)
        
        # Draw flower center
        center_size = max(1, size//3)
        draw.ellipse([
            (x-center_size, y-center_size), 
            (x+center_size, y+center_size)
        ], fill=(255, 220, 0))
        
        # Draw petals radiating outward
        num_petals = random.randint(5, 8)
        for j in range(num_petals):
            petal_angle = j * (360 / num_petals)
            # Petal end position
            petal_end_x = x + size * math.cos(math.radians(petal_angle))
            petal_end_y = y + size * math.sin(math.radians(petal_angle))
            
            # Draw petal as small oval
            petal_size = size // 2
            draw.ellipse([
                (petal_end_x-petal_size, petal_end_y-petal_size), 
                (petal_end_x+petal_size, petal_end_y+petal_size)
            ], fill=color)
    
    # Add some small rocks/pebbles
    rock_colors = [(150, 150, 150), (130, 130, 130), (170, 170, 170)]
    for i in range(80):
        x = random.randint(0, bg_size-1)
        y = random.randint(0, bg_size-1)
        
        size = random.randint(3, 7)
        color = random.choice(rock_colors)
        draw.ellipse([(x, y), (x+size, y+size)], fill=color)
    
    # Create wooden fence boundary
    fence_start = (bg_size - fence_size) // 2
    fence_end = fence_start + fence_size
    
    # Fence properties
    fence_color = (130, 85, 40)  # Richer brown wood color
    fence_width = 16  # Wider fence posts (increased from 8)
    
    # Draw the fence as four sides with posts
    # Top fence - positioned just outside the play area
    draw.rectangle([(fence_start, fence_start - fence_width), (fence_end, fence_start)], fill=fence_color)
    # Right fence - positioned just outside the play area
    draw.rectangle([(fence_end, fence_start), (fence_end + fence_width, fence_end)], fill=fence_color)
    # Bottom fence - positioned just outside the play area
    draw.rectangle([(fence_start, fence_end), (fence_end, fence_end + fence_width)], fill=fence_color)
    # Left fence - positioned just outside the play area
    draw.rectangle([(fence_start - fence_width, fence_start), (fence_start, fence_end)], fill=fence_color)
    
    # Add fence posts at corners and at intervals
    post_size = fence_width * 2  # Larger posts
    post_half = post_size / 2
    
    # Corner posts
    for x, y in [
        (fence_start, fence_start),  # Top-left
        (fence_end, fence_start),    # Top-right
        (fence_end, fence_end),      # Bottom-right
        (fence_start, fence_end)     # Bottom-left
    ]:
        draw.rectangle([
            (x - post_half, y - post_half),
            (x + post_half, y + post_half)
        ], fill=(100, 60, 25))  # Darker wood for posts
    
    # Posts along the fence at regular intervals
    post_interval = 60  # Space between posts
    post_width = fence_width * 1.3
    inner_post_height = fence_width * 1.4
    
    # Top fence posts
    for x in range(fence_start + post_interval, fence_end, post_interval):
        # Taller post extending outward
        draw.rectangle([
            (x - post_width/2, fence_start - fence_width - 5),
            (x + post_width/2, fence_start)
        ], fill=(100, 60, 25))
        
        # Add post caps
        draw.rectangle([
            (x - post_width/2 - 2, fence_start - fence_width - 5 - 3),
            (x + post_width/2 + 2, fence_start - fence_width - 5)
        ], fill=(90, 55, 20))
    
    # Right fence posts
    for y in range(fence_start + post_interval, fence_end, post_interval):
        # Wider post extending outward
        draw.rectangle([
            (fence_end, y - post_width/2),
            (fence_end + fence_width + 5, y + post_width/2)
        ], fill=(100, 60, 25))
        
        # Add post caps
        draw.rectangle([
            (fence_end + fence_width + 5, y - post_width/2 - 2),
            (fence_end + fence_width + 5 + 3, y + post_width/2 + 2)
        ], fill=(90, 55, 20))
    
    # Bottom fence posts
    for x in range(fence_start + post_interval, fence_end, post_interval):
        # Taller post extending outward
        draw.rectangle([
            (x - post_width/2, fence_end),
            (x + post_width/2, fence_end + fence_width + 5)
        ], fill=(100, 60, 25))
        
        # Add post caps
        draw.rectangle([
            (x - post_width/2 - 2, fence_end + fence_width + 5),
            (x + post_width/2 + 2, fence_end + fence_width + 5 + 3)
        ], fill=(90, 55, 20))
    
    # Left fence posts
    for y in range(fence_start + post_interval, fence_end, post_interval):
        # Wider post extending outward
        draw.rectangle([
            (fence_start - fence_width - 5, y - post_width/2),
            (fence_start, y + post_width/2)
        ], fill=(100, 60, 25))
        
        # Add post caps
        draw.rectangle([
            (fence_start - fence_width - 5 - 3, y - post_width/2 - 2),
            (fence_start - fence_width - 5, y + post_width/2 + 2)
        ], fill=(90, 55, 20))
    
    # Add wooden planks in fence sections
    plank_height = fence_width / 3
    plank_gap = 2
    num_planks = 3
    
    # Helper function to add planks to a section of fence
    def add_planks(x1, y1, x2, y2, horizontal=True):
        if horizontal:
            # Horizontal fence sections (top and bottom)
            plank_width = (x2 - x1) / (post_interval / 2)
            for i in range(int((x2 - x1) / plank_width)):
                for j in range(num_planks):
                    start_x = x1 + i * plank_width
                    start_y = y1 + j * (plank_height + plank_gap)
                    if start_y + plank_height <= y2:
                        plank_color = (140, 95, 45) if j % 2 == 0 else (120, 75, 35)
                        draw.rectangle([
                            (start_x, start_y), 
                            (start_x + plank_width - 1, start_y + plank_height)
                        ], fill=plank_color)
        else:
            # Vertical fence sections (left and right)
            plank_width = (y2 - y1) / (post_interval / 2)
            for i in range(int((y2 - y1) / plank_width)):
                for j in range(num_planks):
                    start_y = y1 + i * plank_width
                    start_x = x1 + j * (plank_height + plank_gap)
                    if start_x + plank_height <= x2:
                        plank_color = (140, 95, 45) if j % 2 == 0 else (120, 75, 35)
                        draw.rectangle([
                            (start_x, start_y),
                            (start_x + plank_height, start_y + plank_width - 1)
                        ], fill=plank_color)
    
    # Add planks to each fence section between posts
    for i in range(0, fence_size, post_interval):
        # Skip corners
        if i == 0 or i == fence_size - post_interval:
            continue
            
        # Top fence planks
        add_planks(
            fence_start + i, fence_start - fence_width,
            fence_start + i + post_interval, fence_start,
            horizontal=True
        )
        
        # Bottom fence planks
        add_planks(
            fence_start + i, fence_end,
            fence_start + i + post_interval, fence_end + fence_width,
            horizontal=True
        )
        
        # Left fence planks
        add_planks(
            fence_start - fence_width, fence_start + i,
            fence_start, fence_start + i + post_interval,
            horizontal=False
        )
        
        # Right fence planks
        add_planks(
            fence_end, fence_start + i,
            fence_end + fence_width, fence_start + i + post_interval,
            horizontal=False
        )
    
    # Add wood grain texture to the fence
    wood_grain_color = (90, 60, 30)  # Darker wood color for grain
    
    # Add grain to fence sections (increased from 200 to 400 for more detail)
    for i in range(400):
        section = random.randint(0, 3)
        
        if section == 0:  # Top fence
            x = random.randint(fence_start, fence_end)
            y = random.randint(fence_start - fence_width, fence_start)
            length = random.randint(5, 20)  # Increased length
            draw.line([(x, y), (x + random.randint(-length, length), y)], fill=wood_grain_color, width=1)
        
        elif section == 1:  # Right fence
            x = random.randint(fence_end, fence_end + fence_width)
            y = random.randint(fence_start, fence_end)
            length = random.randint(5, 20)
            draw.line([(x, y), (x, y + random.randint(-length, length))], fill=wood_grain_color, width=1)
        
        elif section == 2:  # Bottom fence
            x = random.randint(fence_start, fence_end)
            y = random.randint(fence_end, fence_end + fence_width)
            length = random.randint(5, 20)
            draw.line([(x, y), (x + random.randint(-length, length), y)], fill=wood_grain_color, width=1)
        
        else:  # Left fence
            x = random.randint(fence_start - fence_width, fence_start)
            y = random.randint(fence_start, fence_end)
            length = random.randint(5, 20)
            draw.line([(x, y), (x, y + random.randint(-length, length))], fill=wood_grain_color, width=1)
    
    # Add knots to the wood for texture (increased from 20 to 40)
    for _ in range(40):
        section = random.randint(0, 3)
        
        if section == 0:  # Top fence
            x = random.randint(fence_start + 20, fence_end - 20)
            y = random.randint(fence_start - fence_width + 3, fence_start - 3)
        elif section == 1:  # Right fence
            x = random.randint(fence_end + 3, fence_end + fence_width - 3)
            y = random.randint(fence_start + 20, fence_end - 20)
        elif section == 2:  # Bottom fence
            x = random.randint(fence_start + 20, fence_end - 20)
            y = random.randint(fence_end + 3, fence_end + fence_width - 3)
        else:  # Left fence
            x = random.randint(fence_start - fence_width + 3, fence_start - 3)
            y = random.randint(fence_start + 20, fence_end - 20)
        
        # Draw the knot
        knot_size = random.randint(3, 6)  # Larger knots
        draw.ellipse([
            (x - knot_size, y - knot_size),
            (x + knot_size, y + knot_size)
        ], fill=(80, 50, 25))
        
        # Add rings around knots
        for r in range(1, 3):
            draw.ellipse([
                (x - knot_size - r, y - knot_size - r),
                (x + knot_size + r, y + knot_size + r)
            ], outline=(100 - r*5, 65 - r*5, 35 - r*5))
    
    return bg

def generate_orientations(base_image):
    return {
        'up': base_image,
        'right': base_image.rotate(-90, expand=True),
        'down': base_image.rotate(180, expand=True),
        'left': base_image.rotate(90, expand=True)
    }

head_images = generate_orientations(create_head_base())
body_images = generate_orientations(create_body_base())
tail_images = generate_orientations(create_tail_base())
apple_image = create_apple()

assets_dir = "snake_assets"
if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)

def swap_direction_name(direction):
    if direction == "up":
        return "down"
    elif direction == "down":
        return "up"
    elif direction == "left":
        return "right"
    elif direction == "right":  
        return "left"
    return direction

for direction, im in head_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"head_{save_direction}.png"))
for direction, im in body_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"body_{save_direction}.png"))
for direction, im in tail_images.items():
    im.save(os.path.join(assets_dir, f"tail_{direction}.png"))
apple_image.save(os.path.join(assets_dir, "apple.png"))

background_image = create_background()
background_image.save(os.path.join(assets_dir, "background.png"))

composite = Image.new("RGBA", (SIZE * 4, SIZE * 4), (255,255,255,0))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(head_images[direction], (i*SIZE, 0))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(body_images[direction], (i*SIZE, SIZE))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(tail_images[direction], (i*SIZE, SIZE*2))
composite.paste(apple_image, (0, SIZE*3))
bg_preview = background_image.resize((SIZE, SIZE), Image.LANCZOS)
composite.paste(bg_preview, (SIZE, SIZE*3))
composite.save(os.path.join(assets_dir, "snake_assets_composite.png"))
composite.show()
