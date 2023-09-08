#include <stdio.h>
#include "allegro.h"
#include "math.h"
#include "stdlib.h"

typedef struct
{
    float x,y,z;
}vec3d;

typedef struct
{
    vec3d p[3];
    int color;
    float ApproxZ;
}t_triangle;

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

void MultiplyMatrixVector(vec3d *i, vec3d *o, float m[4][4])
{
    o->x = i->x * m[0][0] + i->y * m[1][0] + i->z * m[2][0] + m[3][0];
    o->y = i->x * m[0][1] + i->y * m[1][1] + i->z * m[2][1] + m[3][1];
    o->z = i->x * m[0][2] + i->y * m[1][2] + i->z * m[2][2] + m[3][2];
    float w = i->x * m[0][3] + i->y * m[1][3] + i->z * m[2][3] + m[3][3];

    if (w != 0.0f)
    {
        o->x /= w; o->y /= w; o->z /= w;
    }
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

int cmpfunc (void const * a, void const* b) {

    t_triangle *t1=(t_triangle*)a;
    t_triangle *t2=(t_triangle*)b;

    float z1 = (t1->p[0].z + t1->p[1].z + t1->p[2].z) / 3.0f;
    float z2 = (t2->p[0].z + t2->p[1].z + t2->p[2].z) / 3.0f;
    return z1 > z2;
}

int comparer(const void *a, const void *b) {
    const t_triangle *elementA = (const t_triangle *)a;
    const t_triangle *elementB = (const t_triangle *)b;

    // Comparaison basée sur la clé flottante
    if (elementA->ApproxZ < elementB->ApproxZ) return 1;
    if (elementA->ApproxZ > elementB->ApproxZ) return -1;
    return 0;
}

/*
void swap(float Array[], int one, int two) {
    float temp = Array[one];
    Array[one] = Array[two];
    Array[two] = temp;
}

int partition(float Array[], int left, int right) {
    float pivot = Array[right];
    int leftPointer = left - 1;
    int rightPointer = right;
    for (;;) {
        while (Array[++leftPointer] > pivot) {
        }
        while (rightPointer > 0 && Array[--rightPointer] < pivot) {
        }
        if (leftPointer >= rightPointer) {
            break;
        } else {
            swap(Array, leftPointer, rightPointer);
        }
    }
    *//* move pivot to partition point *//*
    swap(Array, leftPointer, right);
    return leftPointer;
}

void Quicksort(float Array[], int left, int right) {
    if (left < right) {
        int PartitionPoint = partition(Array, left, right);
        Quicksort(Array, left, PartitionPoint - 1);
        Quicksort(Array, PartitionPoint + 1, right);
    }
}*/


int main() {
    init();

    FILE *pf;
    pf= fopen("../skull.obj","r");
    if(pf==NULL)
    {
        allegro_message("could not open obj");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    int nbVertices=0,nbTriangle=0;
    char junk;
    char lineLecture[100];

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
    int **tab=malloc(sizeof (int)*nbTriangle);

    for(int i=0; i<nbTriangle;i++)
    {
        tab[i]= malloc(sizeof(int)*3);
    }

    for(int i=0; i<nbTriangle;i++)
    {
        for(int j=0;j<3;j++)
        {
            tab[i][j]=0;/*
            printf("%d",tab[i][j]);*/
        }/*
        printf("\n");*/

    }

    int indiceVerts=0,indiceTab=0;

    /*printf("%s",lecture);*/
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
        /*printf("%f %f %f\n",verts[tab[i][2]-1].x,verts[tab[i][2]-1].y,verts[tab[i][2]-1].z);*/
        tris[i].p[0]=verts[tab[i][0]-1];
        tris[i].p[1]=verts[tab[i][1]-1];
        tris[i].p[2]=verts[tab[i][2]-1];
    }
    /*for(int i=0;i<8;i++)
    {
        printf("%f %f %f\n",verts[i].x,verts[i].y,verts[i].z);
    }*/
/*
    tris[indiceTris].p[0]=verts[f[0]-1];
    tris[indiceTris].p[1]=verts[f[1]-1];
    tris[indiceTris].p[2]=verts[f[2]-1];*/

    /*for(int i=0;i<nbTriangle;i++)
    {
        printf("%f %f %f %f %f %f %f %f %f\n",tris[i].p[0].x,tris[i].p[0].y,tris[i].p[0].z,tris[i].p[1].x,tris[i].p[2].y,tris[i].p[2].z,tris[i].p[2].x,tris[i].p[2].y,tris[i].p[2].z);
    }*/


    t_triangle *triangleToRaster= malloc(sizeof (t_triangle)*nbTriangle);


    float fTheta=0,fPhi=0,fAngleRad=0;
    float dx=0,dy=0,dz=20;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)SCREEN_H / (float)SCREEN_W;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    float matProj[4][4]={0},matRotZ[4][4]={0},matRotX[4][4]={0},matRotY[4][4]={0};
    t_triangle triProjected, triTranslated, triRotatedZ, triRotatedZX,triRotatedZXY;

    vec3d vCamera={0};

    BITMAP *buffer= create_bitmap(SCREEN_W,SCREEN_H);

    /*t_triangle cube[]=
            {
                    // SOUTH
                    { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
                    { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
                    // EAST
                    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
                    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

                    // NORTH
                    { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
                    { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                    // WEST
                    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
                    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

                    // TOP
                    { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
                    { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                    // BOTTOM
                    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
                    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

            };*/

    matProj[0][0] = fAspectRatio * fFovRad;
    matProj[1][1] = fFovRad;
    matProj[2][2] = fFar / (fFar - fNear);
    matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj[2][3] = 1.0f;
    matProj[3][3] = 0.0f;

    float *tabApprox= malloc(sizeof (float)*nbTriangle);


    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);


        if(key [KEY_W])
        {
            fTheta+=0.1f;
        }
        if(key [KEY_E])
        {
            fPhi+=0.1f;
        }

        if(key [KEY_R])
        {
            fAngleRad+=0.1f;
        }


        if(key[KEY_UP])
        {
            dz+=1.0f;
        }
        if(key[KEY_DOWN])
        {
            dz-=1.0f;
        }
        if(key[KEY_T])
        {
            dx+=1.0f;
        }
        if(key[KEY_Y])
        {
            dx-=1.0f;
        }
        if(key[KEY_U])
        {
            dy+=1.0f;
        }
        if(key[KEY_I])
        {
            dy-=0.1f;
        }
        // Rotation Z
        matRotZ[0][0] = cosf(fTheta);
        matRotZ[0][1] = sinf(fTheta);
        matRotZ[1][0] = -sinf(fTheta);
        matRotZ[1][1] = cosf(fTheta);
        matRotZ[2][2] = 1;
        matRotZ[3][3] = 1;

        // Rotation X
        matRotX[0][0] = 1;
        matRotX[1][1] = cosf(fPhi);
        matRotX[1][2] = sinf(fPhi);
        matRotX[2][1] = -sinf(fPhi);
        matRotX[2][2] = cosf(fPhi);
        matRotX[3][3] = 1;

        matRotY[0][0] = cosf(fAngleRad);
        matRotY[0][2] = sinf(fAngleRad);
        matRotY[2][0] = -sinf(fAngleRad);
        matRotY[1][1] = 1.0f;
        matRotY[2][2] = cosf(fAngleRad);
        matRotY[3][3] = 1.0f;

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



            // Rotate in Z-Axis/
            MultiplyMatrixVector(&tris[i].p[0], &triRotatedZ.p[0], matRotZ);
            MultiplyMatrixVector(&tris[i].p[1], &triRotatedZ.p[1], matRotZ);
            MultiplyMatrixVector(&tris[i].p[2], &triRotatedZ.p[2], matRotZ);

            // Rotate in X-Axis
            MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], matRotX);
            MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], matRotX);
            MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], matRotX);

            // Rotate in X-Axis
            MultiplyMatrixVector(&triRotatedZX.p[0], &triRotatedZXY.p[0], matRotY);
            MultiplyMatrixVector(&triRotatedZX.p[1], &triRotatedZXY.p[1], matRotY);
            MultiplyMatrixVector(&triRotatedZX.p[2], &triRotatedZXY.p[2], matRotY);

            // Offset into the screen
            triTranslated = triRotatedZXY;
            triTranslated.p[0].z = triRotatedZXY.p[0].z + dz;
            triTranslated.p[1].z = triRotatedZXY.p[1].z + dz;
            triTranslated.p[2].z = triRotatedZXY.p[2].z + dz;


            triTranslated.p[0].x = triRotatedZXY.p[0].x + dx;
            triTranslated.p[1].x = triRotatedZXY.p[1].x + dx;
            triTranslated.p[2].x = triRotatedZXY.p[2].x + dx;

            triTranslated.p[0].y = triRotatedZXY.p[0].y + dy;
            triTranslated.p[1].y = triRotatedZXY.p[1].y + dy;
            triTranslated.p[2].y = triRotatedZXY.p[2].y + dy;


            // Use Cross-Product to get surface normal
            vec3d normal, line1, line2;
            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            // It's normally normal to normalise the normal
            float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
            normal.x /= l; normal.y /= l; normal.z /= l;

            /*if (normal.z < 0)*/
            if(normal.x * (triTranslated.p[0].x - vCamera.x) +
               normal.y * (triTranslated.p[0].y - vCamera.y) +
               normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
            {


                // Illumination
                vec3d light_direction = { 0.0f, 0.0f, -1.0f };
                float l_light = sqrtf(light_direction.x*light_direction.x + light_direction.y*light_direction.y + light_direction.z*light_direction.z);
                light_direction.x /= l_light; light_direction.y /= l_light; light_direction.z /= l_light;

                // How similar is normal to light direction
                float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

                triTranslated.color= GetColour(dp);

                // Project triangles from 3D --> 2D
                MultiplyMatrixVector(&triTranslated.p[0], &triProjected.p[0], matProj);
                MultiplyMatrixVector(&triTranslated.p[1], &triProjected.p[1], matProj);
                MultiplyMatrixVector(&triTranslated.p[2], &triProjected.p[2], matProj);

                triProjected.color = triTranslated.color;

                // Scale into view
                triProjected.p[0].x += 1.0f;
                triProjected.p[0].y += 1.0f;
                triProjected.p[1].x += 1.0f;
                triProjected.p[1].y += 1.0f;
                triProjected.p[2].x += 1.0f;
                triProjected.p[2].y += 1.0f;
                triProjected.p[0].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[0].y *= 0.5f * (float) SCREEN_H;
                triProjected.p[1].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[1].y *= 0.5f * (float) SCREEN_H;
                triProjected.p[2].x *= 0.5f * (float) SCREEN_W;
                triProjected.p[2].y *= 0.5f * (float) SCREEN_H;
                triangleToRaster[i]=triProjected;
            }
            triangleToRaster[i].ApproxZ=(triangleToRaster[i].p[0].z + triangleToRaster[i].p[1].z + triangleToRaster[i].p[2].z) / 3.0f;
        }

        qsort(triangleToRaster, nbTriangle, sizeof(t_triangle), comparer);


        for(int j=0;j<nbTriangle;j++)
        {
            // Rasterize triangle

            triangle(buffer, (int)triangleToRaster[j].p[0].x, (int)triangleToRaster[j].p[0].y,
                     (int)triangleToRaster[j].p[1].x, (int)triangleToRaster[j].p[1].y,
                     (int)triangleToRaster[j].p[2].x, (int)triangleToRaster[j].p[2].y,
                     (int)triangleToRaster[j].color);

            /*circlefill(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,2, makecol(255,255,255));
            circlefill(buffer,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y,2, makecol(255,255,255));
            circlefill(buffer,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y,2, makecol(255,255,255));

            line(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y, makecol(255,0,0));
            line(buffer,(int)triangleToRaster[j].p[0].x,(int)triangleToRaster[j].p[0].y,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y, makecol(255,0,0));
            line(buffer,(int)triangleToRaster[j].p[2].x,(int)triangleToRaster[j].p[2].y,(int)triangleToRaster[j].p[1].x,(int)triangleToRaster[j].p[1].y, makecol(255,0,0));*/
        }

        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);

    }
    allegro_exit();
    return 0;
}END_OF_MAIN()
