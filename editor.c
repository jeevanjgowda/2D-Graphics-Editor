#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CANVAS_HEIGHT 20
#define CANVAS_WIDTH 60
#define MAX_SHAPES 100

// Define shape types
typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

// Define parameters for each shape
typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;

typedef struct {
    int x1, y1;
    int x2, y2;
} RectParams;

typedef struct {
    int cx, cy;
    int radius;
} CircleParams;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleParams;

// Unified Shape structure
typedef struct {
    int id;
    ShapeType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } params;
} Shape;

// Global variables
char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
Shape shapes[MAX_SHAPES];
int shape_count = 0;
int next_shape_id = 1;

// Function declarations
void clear_canvas();
void display_canvas();
void set_pixel(int x, int y);
void draw_line(int x1, int y1, int x2, int y2);
void draw_rect(int x1, int y1, int x2, int y2);
void draw_circle(int xc, int yc, int r);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void render_all_shapes();

int read_int(const char *prompt, int min_val, int max_val);
void clear_screen();
void add_shape_menu();
void delete_shape_menu();
void modify_shape_menu();
void list_shapes();

int main() {
    clear_canvas();
    
    while (1) {
        clear_screen();
        printf("=== 2D GRAPHICS EDITOR ===\n\n");
        render_all_shapes();
        display_canvas();
        printf("\n");
        
        printf("--- Active Shapes ---\n");
        list_shapes();
        printf("---------------------\n\n");
        
        printf("1. Add a Shape\n");
        printf("2. Delete a Shape\n");
        printf("3. Modify a Shape\n");
        printf("4. Clear Canvas (Delete All)\n");
        printf("5. Exit\n\n");
        
        int choice = read_int("Enter your choice (1-5): ", 1, 5);
        
        switch (choice) {
            case 1:
                add_shape_menu();
                break;
            case 2:
                delete_shape_menu();
                break;
            case 3:
                modify_shape_menu();
                break;
            case 4:
                shape_count = 0;
                printf("Canvas cleared! Press Enter to continue...");
                getchar();
                break;
            case 5:
                printf("Exiting editor. Goodbye!\n");
                return 0;
        }
    }
}

// Clears canvas to underscores
void clear_canvas() {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            canvas[y][x] = '_';
        }
    }
}

// Renders the canvas to the screen with indices
void display_canvas() {
    // Print column header (tens digit)
    printf("   ");
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        if (x % 10 == 0) {
            printf("%d", x / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n   ");
    // Print column header (units digit)
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        printf("%d", x % 10);
    }
    printf("\n");

    // Print top border
    printf("  +");
    for (int x = 0; x < CANVAS_WIDTH; x++) putchar('-');
    printf("+\n");

    // Print rows
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        printf("%2d|", y);
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        printf("|\n");
    }

    // Print bottom border
    printf("  +");
    for (int x = 0; x < CANVAS_WIDTH; x++) putchar('-');
    printf("+\n");
}

// Safely sets a pixel on the canvas
void set_pixel(int x, int y) {
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        canvas[y][x] = '*';
    }
}

// Bresenham's Line Algorithm
void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Draws a rectangle border
void draw_rect(int x1, int y1, int x2, int y2) {
    // Normalize coordinates so x1 <= x2 and y1 <= y2
    if (x1 > x2) { int tmp = x1; x1 = x2; x2 = tmp; }
    if (y1 > y2) { int tmp = y1; y1 = y2; y2 = tmp; }
    
    // Draw top and bottom sides
    for (int x = x1; x <= x2; x++) {
        set_pixel(x, y1);
        set_pixel(x, y2);
    }
    // Draw left and right sides
    for (int y = y1; y <= y2; y++) {
        set_pixel(x1, y);
        set_pixel(x2, y);
    }
}

// Helper for octant points in circle algorithm
static void draw_circle_points(int xc, int yc, int x, int y) {
    set_pixel(xc + x, yc + y);
    set_pixel(xc - x, yc + y);
    set_pixel(xc + x, yc - y);
    set_pixel(xc - x, yc - y);
    set_pixel(xc + y, yc + x);
    set_pixel(xc - y, yc + x);
    set_pixel(xc + y, yc - x);
    set_pixel(xc - y, yc - x);
}

// Midpoint Circle Algorithm
void draw_circle(int xc, int yc, int r) {
    if (r < 0) return;
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    draw_circle_points(xc, yc, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        draw_circle_points(xc, yc, x, y);
    }
}

// Draws a triangle by connecting three points
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

// Re-renders the entire canvas from scratch based on active shapes
void render_all_shapes() {
    clear_canvas();
    for (int i = 0; i < shape_count; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                draw_line(s.params.line.x1, s.params.line.y1, s.params.line.x2, s.params.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rect(s.params.rect.x1, s.params.rect.y1, s.params.rect.x2, s.params.rect.y2);
                break;
            case SHAPE_CIRCLE:
                draw_circle(s.params.circle.cx, s.params.circle.cy, s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(s.params.triangle.x1, s.params.triangle.y1, 
                              s.params.triangle.x2, s.params.triangle.y2, 
                              s.params.triangle.x3, s.params.triangle.y3);
                break;
        }
    }
}

// Robust input function that handles characters and out-of-bounds inputs
int read_int(const char *prompt, int min_val, int max_val) {
    int val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        // Remove trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Parse integer
        char *endptr;
        val = (int)strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        if (val < min_val || val > max_val) {
            printf("Input out of bounds. Must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return val;
    }
}

// Platform-independent screen clear
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

// Lists active shapes with details
void list_shapes() {
    if (shape_count == 0) {
        printf(" (No shapes active)\n");
        return;
    }
    for (int i = 0; i < shape_count; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                printf(" ID %d: Line from (%d,%d) to (%d,%d)\n", 
                       s.id, s.params.line.x1, s.params.line.y1, s.params.line.x2, s.params.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf(" ID %d: Rectangle from (%d,%d) to (%d,%d)\n", 
                       s.id, s.params.rect.x1, s.params.rect.y1, s.params.rect.x2, s.params.rect.y2);
                break;
            case SHAPE_CIRCLE:
                printf(" ID %d: Circle center (%d,%d), radius %d\n", 
                       s.id, s.params.circle.cx, s.params.circle.cy, s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                printf(" ID %d: Triangle (%d,%d)-(%d,%d)-(%d,%d)\n", 
                       s.id, s.params.triangle.x1, s.params.triangle.y1, 
                       s.params.triangle.x2, s.params.triangle.y2, 
                       s.params.triangle.x3, s.params.triangle.y3);
                break;
        }
    }
}

// Menu for adding a shape
void add_shape_menu() {
    if (shape_count >= MAX_SHAPES) {
        printf("Error: Maximum shape limit reached!\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    
    clear_screen();
    printf("=== ADD SHAPE ===\n\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("5. Back to Main Menu\n\n");
    
    int type_choice = read_int("Choose shape type (1-5): ", 1, 5);
    if (type_choice == 5) return;
    
    Shape new_shape;
    new_shape.id = next_shape_id++;
    
    switch (type_choice) {
        case 1:
            new_shape.type = SHAPE_LINE;
            printf("\nEnter line start coordinates:\n");
            new_shape.params.line.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.line.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter line end coordinates:\n");
            new_shape.params.line.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.line.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case 2:
            new_shape.type = SHAPE_RECTANGLE;
            printf("\nEnter top-left corner coordinates:\n");
            new_shape.params.rect.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.rect.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter bottom-right corner coordinates:\n");
            new_shape.params.rect.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.rect.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case 3:
            new_shape.type = SHAPE_CIRCLE;
            printf("\nEnter center coordinates:\n");
            new_shape.params.circle.cx = read_int("  cx (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.circle.cy = read_int("  cy (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.params.circle.radius = read_int("  radius (1-20): ", 1, 20);
            break;
        case 4:
            new_shape.type = SHAPE_TRIANGLE;
            printf("\nEnter vertex 1:\n");
            new_shape.params.triangle.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.triangle.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter vertex 2:\n");
            new_shape.params.triangle.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.triangle.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter vertex 3:\n");
            new_shape.params.triangle.x3 = read_int("  x3 (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.params.triangle.y3 = read_int("  y3 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
    }
    
    shapes[shape_count++] = new_shape;
    printf("\nShape added successfully! Press Enter to continue...");
    getchar();
}

// Menu for deleting a shape
void delete_shape_menu() {
    clear_screen();
    printf("=== DELETE SHAPE ===\n\n");
    list_shapes();
    printf("\n");
    
    if (shape_count == 0) {
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    
    int delete_id = read_int("Enter the ID of the shape to delete (0 to cancel): ", 0, next_shape_id - 1);
    if (delete_id == 0) return;
    
    int index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == delete_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Shape ID not found!\n");
    } else {
        // Shift remaining shapes
        for (int i = index; i < shape_count - 1; i++) {
            shapes[i] = shapes[i + 1];
        }
        shape_count--;
        printf("Shape ID %d deleted successfully!\n", delete_id);
    }
    printf("Press Enter to continue...");
    getchar();
}

// Menu for modifying a shape
void modify_shape_menu() {
    clear_screen();
    printf("=== MODIFY SHAPE ===\n\n");
    list_shapes();
    printf("\n");
    
    if (shape_count == 0) {
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    
    int modify_id = read_int("Enter the ID of the shape to modify (0 to cancel): ", 0, next_shape_id - 1);
    if (modify_id == 0) return;
    
    int index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == modify_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Shape ID not found!\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    
    Shape *s = &shapes[index];
    printf("\nModifying shape ID %d:\n", modify_id);
    
    switch (s->type) {
        case SHAPE_LINE:
            printf("Enter new line start coordinates:\n");
            s->params.line.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.line.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter new line end coordinates:\n");
            s->params.line.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.line.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case SHAPE_RECTANGLE:
            printf("Enter new top-left corner coordinates:\n");
            s->params.rect.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.rect.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter new bottom-right corner coordinates:\n");
            s->params.rect.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.rect.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case SHAPE_CIRCLE:
            printf("Enter new center coordinates:\n");
            s->params.circle.cx = read_int("  cx (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.circle.cy = read_int("  cy (0-19): ", 0, CANVAS_HEIGHT - 1);
            s->params.circle.radius = read_int("  radius (1-20): ", 1, 20);
            break;
        case SHAPE_TRIANGLE:
            printf("Enter new vertex 1:\n");
            s->params.triangle.x1 = read_int("  x1 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.triangle.y1 = read_int("  y1 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter new vertex 2:\n");
            s->params.triangle.x2 = read_int("  x2 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.triangle.y2 = read_int("  y2 (0-19): ", 0, CANVAS_HEIGHT - 1);
            printf("Enter new vertex 3:\n");
            s->params.triangle.x3 = read_int("  x3 (0-59): ", 0, CANVAS_WIDTH - 1);
            s->params.triangle.y3 = read_int("  y3 (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
    }
    
    printf("\nShape modified successfully! Press Enter to continue...");
    getchar();
}
