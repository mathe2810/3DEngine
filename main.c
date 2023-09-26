#include <stdio.h>
#include "allegro.h"
#include "math.h"
#include "stdlib.h"

typedef struct
{
    float x,y,z,w;
}vec3d;

typedef struct
{
    vec3d p[3];
    int color;
    float ApproxZ;
}t_triangle;

typedef struct
{
    float m[4][4] ;
}mat4x4;

#include "stdlib.h"


typedef struct maille
{
    t_triangle data;
    struct maille *next;
}t_maille;


typedef struct file
{
    t_maille *premier;
}t_file;


void handleMalloc(void *var)
{
    if(var==NULL)
    {
        printf("probleme d'allocation\n");
        exit(EXIT_FAILURE);
    }
}


t_file *creation()
{
    t_file *ToReturn= malloc(sizeof (t_file));
    handleMalloc(ToReturn);
    ToReturn->premier=NULL;
    return ToReturn;
}

void enfiler(t_file *file, t_triangle nvTriangle)
{
    t_maille *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->data = nvTriangle;
    nouveau->next = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        t_maille *elementActuel = file->premier;
        while (elementActuel->next != NULL)
        {
            elementActuel = elementActuel->next;
        }
        elementActuel->next = nouveau;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->premier = nouveau;
    }
}

void defiler(t_file *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        t_maille *elementDefile = file->premier;
        file->premier = elementDefile->next;
        free(elementDefile);
    }
}

void afficherFile(t_file *file)
{
    if(file->premier==NULL)
    {
        printf("La file est vide\n");
    }
    else
    {
        t_maille *elementActuel=file->premier;
        while(elementActuel!=NULL)
        {
            printf("%f\n",elementActuel->data.ApproxZ);
            elementActuel=elementActuel->next;
        }
    }
}

void init()
{
    allegro_init();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,1920,1080,0,0)!=0)
    {
        allegro_message("prblm gfx");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    install_keyboard();
    install_mouse();
}

SAMPLE * importeSon(char *nomDeFichier)
{
    SAMPLE *sonARendre= load_sample(nomDeFichier);
    if(!sonARendre)
    {
        allegro_message("ne peut pas ouvrir %s",nomDeFichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return sonARendre;
}


BITMAP * importeImage(char *nomDeFichier)
{
    BITMAP *imageARendre= load_bitmap(nomDeFichier,NULL);
    if(!imageARendre)
    {
        allegro_message("ne peut pas ouvrir %s",nomDeFichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return imageARendre;
}


t_file * fileTrie(t_file*file){}

vec3d initVector()
{
    vec3d toReturn;
    toReturn.x=0;
    toReturn.y=0;
    toReturn.z=0;
    toReturn.w=1;
    return toReturn;
}


vec3d Matrix_MultiplyVector(mat4x4 *m, vec3d *i)
{
vec3d v=initVector();
v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
return v;
}


void MultiplyMatrixVector(vec3d *i, vec3d *o, mat4x4 m)
{
    o->x = i->x * m.m[0][0] + i->y * m.m[1][0] + i->z * m.m[2][0] + m.m[3][0];
    o->y = i->x * m.m[0][1] + i->y * m.m[1][1] + i->z * m.m[2][1] + m.m[3][1];
    o->z = i->x * m.m[0][2] + i->y * m.m[1][2] + i->z * m.m[2][2] + m.m[3][2];
    float w = i->x * m.m[0][3] + i->y * m.m[1][3] + i->z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o->x /= w; o->y /= w; o->z /= w;
    }
}

mat4x4 Matrix_MakeIdentity()
{
    mat4x4 matrix={0};
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationX(float fAngleRad)
{
    mat4x4 matrix={0};
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationY(float fAngleRad)
{
    mat4x4 matrix={0};
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationZ(float fAngleRad)
{
    mat4x4 matrix={0};
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeTranslation(float x, float y, float z)
{
    mat4x4 matrix={0};
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    mat4x4 matrix={0};
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

mat4x4 Matrix_MultiplyMatrix(mat4x4 *m1, mat4x4 *m2)
{
mat4x4 matrix={0};
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++){
            matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
        }
    }
return matrix;
}

vec3d Vector_Add(vec3d *v1, vec3d *v2)
{
    vec3d vToReturn=initVector();
    vToReturn.x = v1->x + v2->x;
    vToReturn.y = v1->y + v2->y;
    vToReturn.z = v1->z + v2->z;
return vToReturn;
}

vec3d Vector_Sub(vec3d *v1, vec3d *v2)
{
    vec3d vToReturn=initVector();
    vToReturn.x = v1->x - v2->x;
    vToReturn.y = v1->y - v2->y;
    vToReturn.z = v1->z - v2->z;
return vToReturn;
}

vec3d Vector_Mul(vec3d *v1, float k)
{
    vec3d vToReturn=initVector();
    vToReturn.x = v1->x * k;
    vToReturn.y = v1->y * k;
    vToReturn.z = v1->z * k;
    return vToReturn;
}

vec3d Vector_Div(vec3d *v1, float k)
{
    vec3d vToReturn=initVector();
    vToReturn.x = v1->x / k;
    vToReturn.y = v1->y / k;
    vToReturn.z = v1->z / k;
    return vToReturn;
}

float Vector_DotProduct(vec3d *v1, vec3d *v2)
{
    return v1->x*v2->x + v1->y*v2->y + v1->z * v2->z;
}

float Vector_Length(vec3d *v)
{
    return sqrtf(Vector_DotProduct(v, v));
}

vec3d Vector_Normalise(vec3d *v)
{
    float l = Vector_Length(v);
    vec3d vToReturn=initVector();
    vToReturn.x = v->x / l;
    vToReturn.y = v->y / l;
    vToReturn.z = v->z / l;
    return vToReturn;
}

vec3d Vector_CrossProduct(vec3d *v1, vec3d *v2)
{
    vec3d v=initVector();
    v.x = v1->y * v2->z - v1->z * v2->y;
    v.y = v1->z * v2->x - v1->x * v2->z;
    v.z = v1->x * v2->y - v1->y * v2->x;
return v;
}

// Return signed shortest distance from point to plane, plane normal must be normalised
float dist(vec3d *p,vec3d *plane_n,vec3d *plane_p)
{
vec3d n = Vector_Normalise(p);
return (plane_n->x * p->x + plane_n->y * p->y + plane_n->z * p->z - Vector_DotProduct(plane_n, plane_p));
}

vec3d Vector_IntersectPlane(vec3d *plane_p, vec3d *plane_n, vec3d *lineStart, vec3d *lineEnd)
{
*plane_n = Vector_Normalise(plane_n);
float plane_d = -Vector_DotProduct(plane_n, plane_p);
float ad = Vector_DotProduct(lineStart, plane_n);
float bd = Vector_DotProduct(lineEnd, plane_n);
float t = (-plane_d - ad) / (bd - ad);
vec3d lineStartToEnd = initVector();
lineStartToEnd = Vector_Sub(lineEnd, lineStart);
vec3d lineToIntersect = initVector();
lineToIntersect = Vector_Mul(&lineStartToEnd, t);
return Vector_Add(lineStart, &lineToIntersect);
}


int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, t_triangle *in_tri, t_triangle *out_tri1, t_triangle *out_tri2)
{
    // Make sure plane normal is indeed normal
    plane_n = Vector_Normalise(&plane_n);

    // Create two temporary storage arrays to classify points either side of plane
    // If distance sign is positive, point lies on "inside" of plane
    vec3d inside_points[3];  int nInsidePointCount = 0;
    vec3d outside_points[3]; int nOutsidePointCount = 0;

    for(int i=0;i<2;i++)
    {
        inside_points[i]=initVector();
        inside_points[i]=initVector();
    }

    // Get signed distance of each point in triangle to plane
    float d0 = dist(&in_tri->p[0],&plane_n,&plane_p);
    float d1 = dist(&in_tri->p[1],&plane_n,&plane_p);
    float d2 = dist(&in_tri->p[2],&plane_n,&plane_p);

    if (d0 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[0]; }
    else { outside_points[nOutsidePointCount++] = in_tri->p[0]; }
    if (d1 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[1]; }
    else { outside_points[nOutsidePointCount++] = in_tri->p[1]; }
    if (d2 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[2]; }
    else { outside_points[nOutsidePointCount++] = in_tri->p[2]; }

    // Now classify triangle points, and break the input triangle into
    // smaller output triangles if required. There are four possible
    // outcomes...

    if (nInsidePointCount == 0)
    {
        // All points lie on the outside of plane, so clip whole triangle
        // It ceases to exist

        return 0; // No returned triangles are valid
    }

    else if (nInsidePointCount == 3)
    {
        // All points lie on the inside of plane, so do nothing
        // and allow the triangle to simply pass through
        *out_tri1 = *in_tri;

        return 1; // Just the one returned original triangle is valid
    }

    else if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
        // Triangle should be clipped. As two points lie outside
        // the plane, the triangle simply becomes a smaller triangle

        // Copy appearance info to new triangle
        out_tri1->color = makecol(255,0,0);

        // The inside point is valid, so keep that...
        out_tri1->p[0] = inside_points[0];

        // but the two new points are at the locations where the
        // original sides of the triangle (lines) intersect with the plane
        out_tri1->p[1] = Vector_IntersectPlane(&plane_p, &plane_n, &inside_points[0], &outside_points[0]);
        out_tri1->p[2] = Vector_IntersectPlane(&plane_p, &plane_n, &inside_points[0], &outside_points[1]);

        return 1; // Return the newly formed single triangle
    }

    else if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        // Triangle should be clipped. As two points lie inside the plane,
        // the clipped triangle becomes a "quad". Fortunately, we can
        // represent a quad with two new triangles

        // Copy appearance info to new triangles
        out_tri1->color = makecol(0,255,0);

        out_tri2->color =  makecol(0,0,255);

        // The first triangle consists of the two inside points and a new
        // point determined by the location where one side of the triangle
        // intersects with the plane
        out_tri1->p[0] = inside_points[0];
        out_tri1->p[1] = inside_points[1];
        out_tri1->p[2] = Vector_IntersectPlane(&plane_p, &plane_n, &inside_points[0], &outside_points[0]);

        // The second triangle is composed of one of he inside points, a
        // new point determined by the intersection of the other side of the
        // triangle and the plane, and the newly created point above
        out_tri2->p[0] = inside_points[1];
        out_tri2->p[1] = out_tri1->p[2];
        out_tri2->p[2] = Vector_IntersectPlane(&plane_p, &plane_n, &inside_points[1], &outside_points[0]);

        return 2; // Return two newly formed triangles which form a quad
    }
    else
    {
        return 0;
    }
}


mat4x4 Matrix_PointAt(vec3d *pos, vec3d *target, vec3d *up)
{
    // Calculate new forward direction
    vec3d newForward=initVector();
    newForward = Vector_Sub(target, pos);
    newForward = Vector_Normalise(&newForward);

// Calculate new Up direction
    vec3d a = Vector_Mul(&newForward, Vector_DotProduct(up, &newForward));
    vec3d newUp = Vector_Sub(up, &a);
    newUp = Vector_Normalise(&newUp);

// New Right direction is easy, its just cross product
    vec3d newRight = Vector_CrossProduct(&newUp, &newForward);

// Construct Dimensioning and Translation Matrix
    mat4x4 matrix={0};
    matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos->x;			matrix.m[3][1] = pos->y;			matrix.m[3][2] = pos->z;			matrix.m[3][3] = 1.0f;
    return matrix;

}


mat4x4 Matrix_QuickInverse(mat4x4 *m) // Only for Rotation/Translation Matrices
{
mat4x4 matrix={0};
matrix.m[0][0] = m->m[0][0]; matrix.m[0][1] = m->m[1][0]; matrix.m[0][2] = m->m[2][0]; matrix.m[0][3] = 0.0f;
matrix.m[1][0] = m->m[0][1]; matrix.m[1][1] = m->m[1][1]; matrix.m[1][2] = m->m[2][1]; matrix.m[1][3] = 0.0f;
matrix.m[2][0] = m->m[0][2]; matrix.m[2][1] = m->m[1][2]; matrix.m[2][2] = m->m[2][2]; matrix.m[2][3] = 0.0f;
matrix.m[3][0] = -(m->m[3][0] * matrix.m[0][0] + m->m[3][1] * matrix.m[1][0] + m->m[3][2] * matrix.m[2][0]);
matrix.m[3][1] = -(m->m[3][0] * matrix.m[0][1] + m->m[3][1] * matrix.m[1][1] + m->m[3][2] * matrix.m[2][1]);
matrix.m[3][2] = -(m->m[3][0] * matrix.m[0][2] + m->m[3][1] * matrix.m[1][2] + m->m[3][2] * matrix.m[2][2]);
matrix.m[3][3] = 1.0f;
return matrix;
}

int GetColour(float lum)
{
    int pixel_bw = (int)(13.0f*lum);
    switch (pixel_bw)
    {
        case 0: return makecol(30,30,30);

        case 1:  return makecol(75,75,75);
        case 2:  return makecol(80,80,80);
        case 3:  return makecol(85,85,85);
        case 4:  return makecol(90,90,90);

        case 5:  return makecol(95,95,95);
        case 6:  return makecol(100,100,100);
        case 7:  return makecol(115,115,115);
        case 8:  return makecol(128,128,128);

        case 9:  return makecol(140,140,140);
        case 10: return makecol(150,150,150);
        case 11: return makecol(190,190,190);
        case 12: return makecol(255,255,255);
        default:
            return makecol(200,200,200);
    }
}

int comparer(const void *a, const void *b) {
    const t_triangle *elementA = (const t_triangle *)a;
    const t_triangle *elementB = (const t_triangle *)b;

    // Comparaison basée sur la clé flottante
    if (elementA->ApproxZ < elementB->ApproxZ) return 1;
    if (elementA->ApproxZ > elementB->ApproxZ) return -1;
    return 0;
}

int myClock(int diffAfterReset)
{
    return clock() - diffAfterReset; // Retourne le temps depuis le dernier reset
}

void myResetClock( int * diffAfterReset)
{
    *diffAfterReset = clock(); // Met le compteur au véritable temps
}


int main() {
    init();

    clock_t tempsDebutOperation,tempsFinOperation;

    FILE *pf;
    pf= fopen("../mapDust.obj","r");
    if(pf==NULL)
    {
        allegro_message("could not open obj");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    int nbVertices=0,nbTriangle=0;
    char junk;
    char lineLecture[100];
    long double fps;

    while(fgets(lineLecture,100,pf)!= NULL)
    {
        if(lineLecture[0]=='v')
        {
            nbVertices++;
        }
        else if(lineLecture[0]=='f')
        {
            nbTriangle++;
        }
    }

    printf("%d %d\n",nbVertices,nbTriangle);


    rewind(pf);

    vec3d *verts=malloc(sizeof (vec3d)*nbVertices);
    t_triangle *tris=malloc(sizeof (t_triangle)*nbTriangle);
    int **tab=malloc(sizeof (int*)*nbTriangle);

    for(int i=0; i<nbTriangle;i++)
    {
        tab[i]= malloc(sizeof(int)*3);
    }

    for(int i=0; i<nbTriangle;i++)
    {
        for(int j=0;j<3;j++)
        {
            tab[i][j]=0;
        }

    }

    int indiceVerts=0,indiceTab=0;

    while(fgets(lineLecture,100,pf)!= NULL){
        if(lineLecture[0]=='v')
        {
            sscanf(lineLecture,"%c %f %f %f",&junk,&verts[indiceVerts].x,&verts[indiceVerts].y,&verts[indiceVerts].z);
            indiceVerts++;

        }
        else if(lineLecture[0]=='f')
        {
            sscanf(lineLecture,"%c %d %d %d",&junk,&tab[indiceTab][0],&tab[indiceTab][1],&tab[indiceTab][2]);
            indiceTab++;
        }
        else
        {

        }


    }

    for(int i=0;i<nbTriangle;i++)
    {
        tris[i].p[0]=verts[tab[i][0]-1];
        tris[i].p[0].w=1;
        tris[i].p[1]=verts[tab[i][1]-1];
        tris[i].p[1].w=1;
        tris[i].p[2]=verts[tab[i][2]-1];
        tris[i].p[2].w=1;
    }


    t_triangle *triangleToRaster= malloc(sizeof (t_triangle)*nbTriangle*3);


    float fTheta=M_PI,fPhi=0,fAngleRad=0;
    float dx=0,dy=0,dz=100;
    float fFov = 90.0f;
    float fYaw=0;
    int compteurTriangle=0;
    t_file *file=creation();

    mat4x4 matProj={0},matRotZ={0},matRotX={0},matRotY={0},matTrans={0},matWorld={0},matRotated={0};
    t_triangle triProjected, triTransformed,triViewed;

    vec3d vCamera={0},vLookDir={0};

    BITMAP *buffer= create_bitmap(SCREEN_W,SCREEN_H);

    matProj= Matrix_MakeProjection(90,(float)SCREEN_H / (float)SCREEN_W,0.1f,1000.0f);

    float *tabApprox= malloc(sizeof (float)*nbTriangle);


    while(!key[KEY_ESC])
    {
        tempsDebutOperation=clock();
        clear_bitmap(buffer);


        if(key [KEY_E])
        {
            fTheta+=10/fps;
        }
        if(key [KEY_R])
        {
            fPhi+=10/fps;
        }

        /*fTheta+=0.01f;
        fPhi+=0.01f*0.5f;*/

        if(key [KEY_T])
        {
            fAngleRad+=10/fps;
        }
        if(key[KEY_Y])
        {
            dz+=100/fps;
        }
        if(key[KEY_U])
        {
            dz-=100/fps;
        }
        if(key[KEY_I])
        {
            dx+=100/fps;
        }
        if(key[KEY_O])
        {
            dx-=100/fps;
        }
        if(key[KEY_P])
        {
            dy+=100/fps;
        }
        if(key[KEY_M])
        {
            dy-=100/fps;
        }

        vec3d vFoward = Vector_Mul(&vLookDir,(float)(10/fps));
        if(key[KEY_W])
        {
            vCamera= Vector_Add(&vCamera,&vFoward);
        }
        if(key[KEY_S])
        {
            vCamera= Vector_Sub(&vCamera,&vFoward);
        }



        if(key[KEY_A])
        {
            fYaw+=1/fps;
        }
        if(key[KEY_D])
        {
            fYaw-=1/fps;
        }



        if(key[KEY_DOWN])
        {
            vCamera.y-=100/fps;
        }
        if(key[KEY_UP])
        {
            vCamera.y+=100/fps;
        }
        if(key[KEY_LEFT])
        {
            vCamera.x+=100/fps;
        }
        if(key[KEY_RIGHT])
        {
            vCamera.x-=100/fps;
        }
        matRotZ= Matrix_MakeRotationZ(fTheta);
        matRotX= Matrix_MakeRotationX(fPhi);
        matRotY= Matrix_MakeRotationY(fAngleRad);

        matTrans= Matrix_MakeTranslation(dx,dy,dz);

        matWorld= Matrix_MakeIdentity();
        matRotated=Matrix_MultiplyMatrix(&matRotZ,&matRotX);
        matRotated=Matrix_MultiplyMatrix(&matRotated,&matRotY);
        matWorld=Matrix_MultiplyMatrix(&matWorld,&matRotated);
        matWorld= Matrix_MultiplyMatrix(&matWorld,&matTrans);


        vec3d vUp={0,1,0};
        vec3d vTarget;

        vTarget.x=0;
        vTarget.y=0;
        vTarget.z=1;


        mat4x4 matCameraRot= Matrix_MakeRotationY(fYaw);

        vLookDir= Matrix_MultiplyVector(&matCameraRot,&vTarget);
        vTarget= Vector_Add(&vCamera,&vLookDir);
        mat4x4 matCamera = Matrix_PointAt(&vCamera,&vTarget,&vUp);

        mat4x4 matView = Matrix_QuickInverse(&matCamera);

        for (int i=0;i<nbTriangle;i++)
        {
            triangleToRaster[i].p[0].x=0;
            triangleToRaster[i].p[0].y=0;
            triangleToRaster[i].p[0].z=0;
            triangleToRaster[i].p[1].x=0;
            triangleToRaster[i].p[1].y=0;
            triangleToRaster[i].p[1].z=0;
            triangleToRaster[i].p[2].x=0;
            triangleToRaster[i].p[2].y=0;
            triangleToRaster[i].p[2].z=0;



            // World Matrix Transform
            triTransformed.p[0] = Matrix_MultiplyVector(&matWorld, &tris[i].p[0]);
            triTransformed.p[1] = Matrix_MultiplyVector(&matWorld, &tris[i].p[1]);
            triTransformed.p[2] = Matrix_MultiplyVector(&matWorld, &tris[i].p[2]);


            // Use Cross-Product to get surface normal
            vec3d normal, line1, line2;

            // Get lines either side of triangle
            line1 = Vector_Sub(&triTransformed.p[1], &triTransformed.p[0]);
            line2 = Vector_Sub(&triTransformed.p[2], &triTransformed.p[0]);

            // Take cross product of lines to get normal to triangle surface
            normal = Vector_CrossProduct(&line1, &line2);

            // You normally need to normalise a normal!
            normal = Vector_Normalise(&normal);

            // Get Ray from triangle to camera
            vec3d vCameraRay = Vector_Sub(&triTransformed.p[0],&vCamera);

            // If ray is aligned with normal, then triangle is visible
            if (Vector_DotProduct(&normal, &vCameraRay) < 0.0f) {
                // Illumination
                vec3d light_direction = {1.0f, 1.0f, -1.0f};
                light_direction = Vector_Normalise(&light_direction);

                // How "aligned" are light direction and triangle surface normal?
                float dp = fmax(0.1f, Vector_DotProduct(&light_direction, &normal));

                // Choose console colours as required (much easier with RGB)
                triTransformed.color = GetColour(dp);


                // Convert World Space --> View Space
                triViewed.p[0] = Matrix_MultiplyVector(&matView, &triTransformed.p[0]);
                triViewed.p[1] = Matrix_MultiplyVector(&matView, &triTransformed.p[1]);
                triViewed.p[2] = Matrix_MultiplyVector(&matView, &triTransformed.p[2]);

                triViewed.color = triTransformed.color;

                // Clip Viewed Triangle against near plane, this could form two additional triangles:

                int nClippedTriangles = 0;
                t_triangle clipped[2];

                vec3d vecTmp = {0.0f, 0.0f, 0.1f};
                vec3d vecTmp2 = {0.0f, 0.0f, 1.0f};
                nClippedTriangles = Triangle_ClipAgainstPlane(vecTmp, vecTmp2, &triViewed, &clipped[0], &clipped[1]);
                compteurTriangle += nClippedTriangles;

                for (int n = 0; n < nClippedTriangles; n++) {
                    // Project triangles from 3D --> 2D
                    triProjected.p[0] = Matrix_MultiplyVector(&matProj, &clipped[n].p[0]);
                    triProjected.p[1] = Matrix_MultiplyVector(&matProj, &clipped[n].p[1]);
                    triProjected.p[2] = Matrix_MultiplyVector(&matProj, &clipped[n].p[2]);
                    triProjected.color = clipped[n].color;

                    // Scale into view, we moved the normalising into cartesian space
                    // out of the matrix.vector function from the previous videos, so
                    // do this manually
                    triProjected.p[0] = Vector_Div(&triProjected.p[0], triProjected.p[0].w);
                    triProjected.p[1] = Vector_Div(&triProjected.p[1], triProjected.p[1].w);
                    triProjected.p[2] = Vector_Div(&triProjected.p[2], triProjected.p[2].w);

                    // Offset verts into visible normalised space
                    vec3d vOffsetView = {1, 1, 0};
                    triProjected.p[0] = Vector_Add(&triProjected.p[0], &vOffsetView);
                    triProjected.p[1] = Vector_Add(&triProjected.p[1], &vOffsetView);
                    triProjected.p[2] = Vector_Add(&triProjected.p[2], &vOffsetView);
                    triProjected.p[0].x *= 0.5f * (float) SCREEN_W;
                    triProjected.p[0].y *= 0.5f * (float) SCREEN_H;
                    triProjected.p[1].x *= 0.5f * (float) SCREEN_W;
                    triProjected.p[1].y *= 0.5f * (float) SCREEN_H;
                    triProjected.p[2].x *= 0.5f * (float) SCREEN_W;
                    triProjected.p[2].y *= 0.5f * (float) SCREEN_H;
                    triProjected.ApproxZ = (triProjected.p[0].z + triProjected.p[1].z + triProjected.p[2].z) / 3.0f;
                    triangleToRaster[i+n]=triProjected;
                }

               /* // Project triangles from 3D --> 2D
                triProjected.p[0] = Matrix_MultiplyVector(&matProj, &triViewed.p[0]);
                triProjected.p[1] = Matrix_MultiplyVector(&matProj, &triViewed.p[1]);
                triProjected.p[2] = Matrix_MultiplyVector(&matProj, &triViewed.p[2]);

                triProjected.p[0] = Vector_Div(&triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = Vector_Div(&triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = Vector_Div(&triProjected.p[2], triProjected.p[2].w);

                triProjected.color = triTransformed.color;

                // Scale into view
                // Offset verts into visible normalised space
                vec3d vOffsetView = {1, 1, 0};
                triProjected.p[0] = Vector_Add(&triProjected.p[0], &vOffsetView);
                triProjected.p[1] = Vector_Add(&triProjected.p[1], &vOffsetView);
                triProjected.p[2] = Vector_Add(&triProjected.p[2], &vOffsetView);
                triProjected.p[0].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[0].y *= 0.5f * (float) SCREEN_H;
                triProjected.p[1].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[1].y *= 0.5f * (float) SCREEN_H;
                triProjected.p[2].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[2].y *= 0.5f * (float) SCREEN_H;
                triProjected.ApproxZ = (triProjected.p[0].z + triProjected.p[1].z + triProjected.p[2].z) / 3.0f;
                //enfiler(file, triProjected);
                triangleToRaster[i] = triProjected;
                triangleToRaster[i].ApproxZ =(triangleToRaster[i].p[0].z + triangleToRaster[i].p[1].z + triangleToRaster[i].p[2].z) / 3.0f;*/
            }
        }

        qsort(triangleToRaster, nbTriangle, sizeof(t_triangle), comparer);

       /* qsort(file,nbTriangle,sizeof(t_file),comparer);*/

        t_maille *maille=file->premier;

        for(int j=0;j<nbTriangle;j++)/*
        for(maille;maille!=NULL;maille=maille->next)*/
        {

            // Rasterize triangle

            triangle(buffer, (int)triangleToRaster[j].p[0].x, (int)triangleToRaster[j].p[0].y,
                     (int)triangleToRaster[j].p[1].x, (int)triangleToRaster[j].p[1].y,
                     (int)triangleToRaster[j].p[2].x, (int)triangleToRaster[j].p[2].y,
                     (int)triangleToRaster[j].color);
            /*triangle(buffer, (int)maille->data.p[0].x, (int)maille->data.p[0].y,
                     (int)maille->data.p[1].x, (int)maille->data.p[1].y,
                     (int)maille->data.p[2].x, (int)maille->data.p[2].y,
                     (int)maille->data.color);*/


            circlefill(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,2, makecol(255,255,255));
            circlefill(buffer,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y,2, makecol(255,255,255));
            circlefill(buffer,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y,2, makecol(255,255,255));

            line(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y, makecol(255,0,0));
            line(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y, makecol(255,0,0));
            line(buffer,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y, makecol(255,0,0));
        }
        /*for(int j=0;j<nbTriangle;j++)
        {
            defiler(file);
        }*/


        tempsFinOperation=clock();
        if((tempsFinOperation-tempsDebutOperation)!=0)
        {
           fps=(double)1000/(tempsFinOperation-tempsDebutOperation);
        }
        textprintf_ex(buffer,font,100,100, makecol(255,0,0),-1,"%Lf %s",(long double)(fps),"fps");
        textprintf_ex(buffer,font,100,200, makecol(255,0,0),-1,"%d %s",compteurTriangle,"triangle dessiné");
        compteurTriangle=0;
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);


    }
    allegro_exit();
    return 0;
}END_OF_MAIN()
