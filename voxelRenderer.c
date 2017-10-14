#include "voxelRenderer.h"

unsigned int voxColors[256] = {
    0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
	0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
	0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
	0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
	0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
	0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
	0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
	0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
	0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
	0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
	0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
	0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
	0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
	0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
	0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
	0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};



extern const int GAME_SCREEN_WIDTH;
extern const int GAME_SCREEN_HEIGHT;
extern double deltaTime;

Pixel *screen = NULL;
Uint16 *depth = NULL;

Pixel *cube;
Uint8 *cubeDepth;
int cubeWidth = 0;
int cubeHeight = 0;
Vector3 cameraPosition;

void MoveCamera(float x, float y, float z){
    cameraPosition.x +=x*deltaTime;
    cameraPosition.y +=y*deltaTime;
    cameraPosition.z +=z*deltaTime;
    //printf("CamPos: |%2.1f|%2.1f|%2.1f|\n",cameraPosition.x,cameraPosition.y,cameraPosition.z);
}

void ClearScreen(){
    int y,x,cp = 0;
    for(y=0;y<GAME_SCREEN_HEIGHT;y++){
        for(x=0;x<GAME_SCREEN_WIDTH;x++){
            screen[cp].r = 0;
            screen[cp].g = 38;
            screen[cp].b = 75;
            depth[cp] = 0;
            cp++;
        }
    }
}

void PostProcess(){
    int y,x,cp = 0;
    int useOutline = 1;
    float outline = 0.54;

    for(y=0;y<GAME_SCREEN_HEIGHT;y++){
        for(x=0;x<GAME_SCREEN_WIDTH;x++){

            if(useOutline){
                //Outline effect
                if(depth[cp]!=0 && cp%GAME_SCREEN_WIDTH !=0 && cp%GAME_SCREEN_WIDTH !=GAME_SCREEN_WIDTH-1){
                    if(cp-1>0){
                        if((depth[cp-1]-depth[cp])<-10 || depth[cp-1] == 0){
                            screen[cp-1].r = screen[cp].r*outline;
                            screen[cp-1].g = screen[cp].g*outline;
                            screen[cp-1].b = screen[cp].b*outline;
                        }
                    }
                    if(cp+1<GAME_SCREEN_HEIGHT*GAME_SCREEN_WIDTH){
                        if((depth[cp+1]-depth[cp])<-10 || depth[cp+1] == 0){
                            screen[cp+1].r = screen[cp].r*outline;
                            screen[cp+1].g = screen[cp].g*outline;
                            screen[cp+1].b = screen[cp].b*outline;
                        }
                    }
                    if(cp-GAME_SCREEN_WIDTH>0){
                        if((depth[cp-GAME_SCREEN_WIDTH]-depth[cp])<-10 || depth[cp-GAME_SCREEN_WIDTH] == 0){
                            screen[cp-GAME_SCREEN_WIDTH].r = screen[cp].r*outline;
                            screen[cp-GAME_SCREEN_WIDTH].g = screen[cp].g*outline;
                            screen[cp-GAME_SCREEN_WIDTH].b = screen[cp].b*outline;
                        }
                    }
                    if(cp+GAME_SCREEN_WIDTH<GAME_SCREEN_HEIGHT*GAME_SCREEN_WIDTH){
                        if((depth[cp+GAME_SCREEN_WIDTH]-depth[cp])<-10 || depth[cp+GAME_SCREEN_WIDTH] == 0){
                            screen[cp+GAME_SCREEN_WIDTH].r = screen[cp].r*outline;
                            screen[cp+GAME_SCREEN_WIDTH].g = screen[cp].g*outline;
                            screen[cp+GAME_SCREEN_WIDTH].b = screen[cp].b*outline;
                        }
                    }
                }
            }
            cp++;
        }
    }
}
void *RenderThread(void *arguments){
    RendererArguments *args = arguments;

	VoxelObject **objs = args->objs;
	unsigned int numObjs = args->numObjs;
	VoxelObject **shadowCasters = args->shadowCasters;
	unsigned int numCasters = args->numCasters;
    int i,endLoop = 0;
    pthread_t tID;

    for(i=0; i<numObjs; i++){
        if(objs[i]->enabled){
            if(objs[i]->maxDimension >= 100 && numObjs-(i+1)>0){
				RendererArguments renderArguments = {objs+(i+1),numObjs-(i+1),shadowCasters,numCasters};
				pthread_create(&tID, NULL, &RenderThread, (void *)&renderArguments);
                endLoop = 1;
            }
            if(objs[i]->modificationStartZ >=0){
                CalculateRendered(objs[i]);
                CalculateLighting(objs[i]);

                objs[i]->modificationStartX = -1;
                objs[i]->modificationEndX = -1;

                objs[i]->modificationStartY = -1;
                objs[i]->modificationEndY = -1;

                objs[i]->modificationStartZ = -1;
                objs[i]->modificationEndZ = -1;

            }
            RenderObject(objs[i]);
            if(shadowCasters!=NULL){
                for(int j=0;j<numCasters;j++){
                    CalculateShadow(objs[i],shadowCasters[j]);
                }
            }
            if(endLoop){
                pthread_join(tID, NULL);
                break;
            }
        }
    }
    return NULL;
}

void InitRenderer(Uint16 *dpth){
    depth = dpth;

    if(depth == NULL){
        printf("\nError initializing renderer!\n");
        system("pause");
    }

    cameraPosition = (Vector3){-GAME_SCREEN_WIDTH/2,-GAME_SCREEN_HEIGHT/2,0};

    SDL_Surface *cubeimg = IMG_Load("Textures/cube.png");
    SDL_Surface *cubeDepthimg = IMG_Load("Textures/cubeDepth.png");

    int bpp = cubeimg->format->BytesPerPixel;
    cubeWidth = cubeimg->w;
    cubeHeight = cubeimg->h;

    cube = (Pixel*)calloc(cubeimg->w*cubeimg->h,sizeof(Pixel));
    cubeDepth = (Uint8 *)calloc(cubeimg->w*cubeimg->h,sizeof(Uint8));

    int i;
    for(i=0;i<cubeimg->w*cubeimg->h;i++){
        
        Uint8* p = (Uint8*) cubeimg->pixels + i * bpp;
        Uint32 pixelColor = *(Uint32*)p;

        SDL_GetRGBA(pixelColor,cubeimg->format,&cube[i].r,&cube[i].g,&cube[i].b,&cube[i].a);

        Uint8* d = (Uint8*) cubeDepthimg->pixels + i * bpp;
        Uint32 depthColor = *(Uint32*)d;
        Uint8 discart;
        Uint8 depth;
        SDL_GetRGBA(depthColor,cubeDepthimg->format,&depth,&discart,&discart,&discart);

        cubeDepth[i] = depth/32;
    }
}

void UpdateScreenPointer(Pixel* scrn){
    screen = scrn;
}

void FreeRenderer(){
    free(depth);
    free(cube);
    free(cubeDepth);
 }

void RenderObject(VoxelObject *obj){

    unsigned int color = 0;

    int x,y,z,i,px,py,zp,startz,aux,nv,cp = 0,colorIndex,edgeIndx,lightIndx,useRot = 0;
    Uint16 voxeld,pixeld;
    Pixel pixel;
    int halfDimX = obj->dimension[0]/2.0, halfDimY = obj->dimension[1]/2.0,halfDimZ = obj->dimension[2]/2.0;
    float rx,ry,rz;
    float sinx = 1,cosx = 0;
    float siny = 1,cosy = 0;
    float sinz = 1,cosz = 0;
    //Termos que multiplicam as posicoes na rotacao
    float rxt1 = 1, rxt2 = 1, rxt3 = 1;
    float ryt1 = 1, ryt2 = 1, ryt3 = 1;
    float rzt1 = 1, rzt2 = 1;

    if(obj->rotation.x != 0.0f || obj->rotation.y != 0.0f || obj->rotation.z != 0.0f){
        useRot = 1;
        sinx = sin(obj->rotation.x * PI_OVER_180);
        cosx = cos(obj->rotation.x * PI_OVER_180);

        siny = sin(obj->rotation.y * PI_OVER_180);
        cosy = cos(obj->rotation.y * PI_OVER_180);
        
        sinz = sin(obj->rotation.z * PI_OVER_180);
        cosz = cos(obj->rotation.z * PI_OVER_180);

        //Pre calculo dos termos
        rxt1 = cosy*cosz; rxt2 = (cosz*sinx*siny - cosx*sinz); rxt3 = (cosx*cosz*siny + sinx*sinz);
        ryt1 = cosy*sinz; ryt2 = (cosx*siny*sinz - cosz*sinx); ryt3 = (cosx*cosz + sinx*siny*sinz);
        rzt1 = cosx*cosy; rzt2 = sinx*cosy;
    }
    const float edge = 0.80;
    const float base = 0.75;
    const float crease = 0.70;
    const float sunlight = 1.42;
    const float shadow = 0.79;

    double illuminFrac;
    float lightVal,edgeVal,heightVal;
    float r,g,b;
    startz = (obj->dimension[2]-1);
    

    //Checagem se fora da tela
    //> Desabilitada por enquanto, não utilizo elementos fora da tela ainda
    //if( /*Esquerda*/ ((obj->maxDimension+obj->position.x)-(obj->position.y))*2 + roundf(-cameraPosition.x) < 0 ||
    //    /*Direita*/  ((obj->position.x)-(obj->maxDimension+obj->position.y))*2 + roundf(-cameraPosition.x) > GAME_SCREEN_WIDTH ||
    //    /*Acima*/    ((obj->maxDimension+obj->position.x)+(obj->maxDimension+obj->position.y)) + roundf(-cameraPosition.y) < 0 ||
    //    /*Abaixo*/   ((obj->position.x)+(obj->position.y)) -(obj->maxDimension*2) + roundf(-cameraPosition.y) > GAME_SCREEN_HEIGHT
    //){
    //    return;
    //}
    
    for(z=startz; z!=0; z--){
        heightVal = clamp(((1.0+(((z+obj->position.z+cameraPosition.z)*0.5))/128)),0,1.4);

        nv = obj->render[z][0];
        for(i = 1; i <= nv ; i++){
            
            x = (obj->render[z][i] & 127);
            y = ((obj->render[z][i]>>7) & 127);

            colorIndex = (x) + ((z)) * obj->maxDimension + (y) * obj->maxDimension * obj->maxDimension;
            color = voxColors[obj->model[colorIndex]];

            //Aplica rotação na posição do voxel
            if(useRot){
                x -= halfDimX;
                y -= halfDimY;
                z -= halfDimZ;

                rx = x*rxt1 + y*rxt2 + z*rxt3;
                ry = x*ryt1 + z*ryt2 + y*ryt3;
                rz = z*rzt1 + y*rzt2 - x*siny;

                rx += halfDimX;
                ry += halfDimY;
                rz += halfDimZ;

                z += halfDimZ;
            }else{
                rx = x;
                ry = y;
                rz = z;
            }
            rx = roundf(rx);
            ry = roundf(ry);
            rz = roundf(rz);
            //Clipping do objeto quando fora da faixa de 0 a 255
            zp = rz + roundf(obj->position.z+cameraPosition.z);
            if(zp<0 || zp>255) continue;
            
            //Obtém o nivel de iluminação do voxel e multiplica pela sombra dinâmica
            lightIndx = (obj->lighting[colorIndex] & 6)>>1;
            lightIndx *=obj->lighting[colorIndex] & 1;
            //Reseta a sombra dinâmica para 1 (sem sombra), para ser recalculada no prox frame 
            obj->lighting[colorIndex] |= 1;

            //Adiciona iluminação leve nas bordas
            edgeIndx = obj->lighting[colorIndex]>>3;
            lightVal = lightIndx == 1? 1:(lightIndx >= 2? sunlight:shadow);
            edgeVal = (edgeIndx<5? edge:edgeIndx == 5? base:crease);

            //Multiplica iluminações e já coloca a conversão da cor de (0,256) para (0,1)
            illuminFrac = lightVal * edgeVal * heightVal * ONE_OVER_256;

            //Transforma a cor de um Int16 para cada um dos componentes RGB
            voxeld = zp*8;
            r = clamp((color & 255)*illuminFrac,0,1);
            color = (color>>8);
            g = clamp((color & 255)*illuminFrac,0,1);
            color = (color>>8);
            b = clamp((color & 255)*illuminFrac,0,1);

            //Projeção das posições de 3 dimensões para duas na tela
            py = ((rx+obj->position.x)+(ry+obj->position.y)) -(zp*2) + roundf(-cameraPosition.y);
            px = ((rx+obj->position.x)-(ry+obj->position.y))*2 + roundf(-cameraPosition.x);

            //Pula os voxels que estiverem fora da tela
            if(py+cubeHeight<0 || py>GAME_SCREEN_HEIGHT) continue;
            if(px+cubeWidth<0  || py>GAME_SCREEN_WIDTH ) continue;

            //Renderização do sprite do voxel
            int cx,cy;
            for(cy=0;cy<cubeHeight;cy++){
                for(cx=0;cx<cubeWidth;cx++){
                    //Obtém cor da posição atual no sprite
                    pixel = cube[cx+cy*cubeWidth];
                    //Descarta pixels transparentes
                    if(pixel.a==0) continue;

                    //Pula os pixels que estiverem fora da tela
                    cp = py+cy;
                    cp = cp>=GAME_SCREEN_HEIGHT? -1: (cp<0? -1:cp*GAME_SCREEN_WIDTH);
                    if(cp <0){
                        continue;
                    }
                    aux = px+cx;
                    cp = (aux)>=GAME_SCREEN_WIDTH? -1: ((aux)<0? -1:cp + (aux));
                    if(cp <0){
                        continue;
                    }

                    //Escreve pixels na tela e no depth se estiver na frente do atual
                    pixeld = voxeld+cubeDepth[cx+cy*cubeWidth];
                    if(pixeld > depth[cp]){
                                          
                        screen[cp].r = pixel.r*r;
                        screen[cp].g = pixel.g*g;
                        screen[cp].b = pixel.b*b;
                        depth[cp] = pixeld;
                    }
                }
            }
        }
    }
}

void CalculateRendered(VoxelObject *obj){
    if(obj->modificationStartZ <0 || obj->modificationEndZ <0 ){
        return;
    }
    int x,y,z,index,dir,occ,occPixel = 0,occUp,occLeft,occDown;
    for(z = obj->modificationStartZ; z<=obj->modificationEndZ ;z++){
        obj->render[z][0]=0;
        for(y = obj->dimension[1]-1; y>=0; y--){
            for(x = obj->dimension[0]-1; x>=0; x--){
                occ = 0;
                occUp   = 0;
                occLeft = 0;
                occDown = 0;

                index = (x + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                if(obj->model[index]!=0){
                    if(x!=0 && x<obj->maxDimension-1 && y!=0 && y<obj->maxDimension-1 && z!=0 && z<obj->maxDimension-1){
                        dir = (x + (z+1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);//0 0 1
                        if(obj->model[dir]!=0){
                            occ++; 
                            occUp=1;
                        }
                        dir = (x + (z-1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);//0 0 -1
                        if(obj->model[dir]!=0){
                            occ++;
                            occDown = 1;
                        }
                        dir = (x + z * obj->maxDimension + (y+1) * obj->maxDimension * obj->maxDimension);//0 1 0
                        if(obj->model[dir]!=0){
                            occ++;
                        }
                        dir = (x + z * obj->maxDimension + (y-1) * obj->maxDimension * obj->maxDimension);//0 -1 0 
                        if(obj->model[dir]!=0){
                            occ++;
                        }
                        dir = ( (x+1) + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);//1 0 0
                        if(obj->model[dir]!=0){
                            occ++;
                        }
                        dir = ( (x-1) + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);//1 0 0 
                        if(obj->model[dir]!=0){
                            occ++;
                            occLeft = 1;
                        }
                    }
                    if(occ!=6){
                        if((occLeft && occDown && occUp) || y == obj->dimension[1]-1){
                            occPixel = 0;
                        }else{
                            if(!occLeft && !occDown && occUp){
                                occPixel = 1;
                            }
                            if(!occLeft && occDown && occUp){
                                occPixel = 1;
                            }
                            if(!occDown && !occUp && x>0){
                                occPixel = 2;
                            }
                            if(occLeft && !occDown){
                                occPixel = 2;
                            }
                        }
                        obj->render[z][0]++;
                        obj->render[z][(int)obj->render[z][0]] = (unsigned short int)((occPixel<<14) | ( y << 7) | x);
                    }
                }
            }
        }
    }
}


void CalculateLighting(VoxelObject *obj){

    int y,x,z,index,dir;
    int occlusion,lightAir,lightBlock;

    int zstart = obj->dimension[2]-1;
    int lightFinal;
    for(y=obj->modificationStartY; y<obj->modificationEndY; y++){
        for(x=obj->modificationStartX; x<obj->modificationEndX; x++){

            //Define a luz no topo do objeto, que é transportado para baixo a cada iteração em z
            lightAir = 1;
            lightBlock = 1;

            for(z=zstart; z!=0; z--){
                occlusion = 0;
                index = (x + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                
                if(obj->model[index]!=0){
                    if(z<obj->dimension[2]-1){ //Up
                        dir = (x + (z+1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0? 0:1;
                        //Ilumina o bloco caso o bloco acima seja vazio (com luz ou sombra), se não, mantém a cor
                        lightBlock = obj->model[dir]==0? lightAir*2:1;
                    }else{
                        lightBlock = 2;
                        occlusion = 3;
                    }
                    if(z>0){ //Down
                        dir = (x + (z-1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0?  0:1;
                    }else{
                        occlusion = 3;
                    }
                    if(x>0){ //Left
                        dir = ((x-1) + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0? 0:1;
                    }else{
                        occlusion = 2;
                    }
                    if(x<obj->dimension[0]-1){ //Right
                        dir = ((x+1) + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0?  0:1;
                    }else{
                        occlusion = 3;
                    }
                    if(y<obj->dimension[1]-1){ //Front
                        dir = (x + z * obj->maxDimension + (y+1) * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0?  0:1;
                    }else{
                        occlusion = 3;
                    }
                    if(y>0){ //Back
                        dir = (x + z * obj->maxDimension + (y-1) * obj->maxDimension * obj->maxDimension);
                        occlusion += obj->model[dir]==0?  0:1;
                    }else{
                        occlusion = 3;
                    }
                    lightFinal = lightBlock;
                }else{
                    if(z+1<obj->maxDimension){
                        dir = (x + (z+1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        if(obj->model[dir]!=0){
                            lightAir = 0;
                        }
                    }
                    lightFinal = lightAir;
                }

                //lighting 8bits  [1-Empty] [3-Occlusion][2-Direct Light(2), Ambient(1) and self shadow(0)] [1-Shadow from caster]
                obj->lighting[index] = (unsigned char)( (((occlusion & 255)<<3)|((lightFinal & 3)<<1)) | (obj->lighting[index] & 1) );
            }
        }
    }
    /*for(y=obj->modificationStartY; y<obj->modificationEndY; y++){
        for(x=obj->modificationStartX; x<obj->modificationEndX; x++){
            for(z=zstart; z>=0; z--){
                index = (x + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                if(obj->model[index]==0){
                    PointLight(obj,x,y,z,3);
                }
            }
        }
    }*/
}

/*void PointLight(VoxelObject *obj,int x, int y, int z,int radius){
    int px,py,pz;

    px = x;
    py = y; 
    pz = z;

    int startx,endx,starty,endy,startz,endz;
    int ix,iy,iz,index,total = 0;

    startx = px-radius <0? 0:px-radius;
    starty = py-radius <0? 0:py-radius;
    startz = pz-radius <0? 0:pz-radius;

    endx = px+radius>obj->maxDimension? obj->maxDimension : px+radius;
    endy = py+radius>obj->maxDimension? obj->maxDimension : py+radius;
    endz = pz+radius>obj->maxDimension? obj->maxDimension : pz+radius;

    for(ix = startx;ix<endx;ix++){
        for(iy = starty;iy<endy;iy++){
            for(iz = startz;iz<endz;iz++){
                int randRadius = radius+0;//(rand() % 3);
                if( ((ix-px)*(ix-px))+((iy-py)*(iy-py))+((iz-pz)*(iz-pz)) <= (randRadius*randRadius)){
                    total++;
                    index = (ix) + (iz) * obj->maxDimension + (iy) * obj->maxDimension * obj->maxDimension;

                    //lighting => 8bits  [2-Empty] [3-Occlusion][2-Point Light(3) Direct Light(2), Ambient(1) and self shadow(0)] [1-Shadow from caster]
                    obj->lighting[index] = (unsigned char)(obj->lighting[index] | 0b00000110);
                }
            }
        }   
    }
}*/

void CalculateShadow(VoxelObject *obj,VoxelObject *shadowCaster){

    int y,x,z,o,index,dir,cx,cy,cz,useRot = 0;

    int halfDimX = shadowCaster->dimension[0]/2.0, halfDimY = shadowCaster->dimension[1]/2.0,halfDimZ = shadowCaster->dimension[2]/2.0;
    float rx,ry,rz;
    float sinx = 1,cosx = 0;
    float siny = 1,cosy = 0;
    float sinz = 1,cosz = 0;
    //Revisitar essa parte, projeção inverte em certos ângulos
    if(shadowCaster->rotation.x != 0.0f || shadowCaster->rotation.y != 0.0f || shadowCaster->rotation.z != 0.0f){
        useRot = 1;
        sinx = -sin(shadowCaster->rotation.x * PI_OVER_180);
        cosx = cos(shadowCaster->rotation.x * PI_OVER_180);

        siny = -sin(shadowCaster->rotation.y * PI_OVER_180);
        cosy = cos(shadowCaster->rotation.y * PI_OVER_180);
        
        sinz = -sin(shadowCaster->rotation.z * PI_OVER_180);
        cosz = cos(shadowCaster->rotation.z * PI_OVER_180);
    }

    
    int startx = shadowCaster->position.x-obj->position.x - shadowCaster->dimension[0]*0.2f;
    int starty = shadowCaster->position.y-obj->position.y - shadowCaster->dimension[1]*0.2f;
    int startz = (shadowCaster->position.z-obj->position.z)+shadowCaster->dimension[2];

    int endx = startx + shadowCaster->dimension[0]*1.5f;
    int endy = starty + shadowCaster->dimension[1]*1.5f;
    if(endx<0 || endy<0 ){
        return;
    }

    startx = startx <0? 0:startx;
    starty = starty <0? 0:starty;
    startz = startz <0? 0:startz;

    startz = startz >obj->maxDimension? obj->maxDimension:startz;
    endx = endx>obj->dimension[0]? obj->dimension[0] : endx;
    endy = endy>obj->dimension[1]? obj->dimension[1] : endy;
        
    int shadowVal,finalShadow = 1;
    for(y=starty; y<endy; y++){
        for(x=startx; x<endx; x++){

            shadowVal = 1;

            for(z=startz; z!=0; z--){
                index = (x + z * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                if(obj->model[index]==0){
                    if(shadowCaster->enabled == 0){
                        continue;
                    }
                    cx = x-shadowCaster->position.x+obj->position.x;
                    cy = y-shadowCaster->position.y+obj->position.y;
                    cz = z-shadowCaster->position.z+obj->position.z;

                    if(useRot==1){
                        cx -= halfDimX;
                        cy -= halfDimY;
                        cz -= halfDimZ;
        
                        rx = cx*cosy*cosz + cy*(cosz*sinx*siny - cosx*sinz) + cz*(cosx*cosz*siny + sinx*sinz);
                        ry = cx*cosy*sinz + cz*(cosx*siny*sinz - cosz*sinx) + cy*(cosx*cosz + sinx*siny*sinz);
                        rz = cz*cosx*cosy + cy*sinx*cosy - cx*siny;                      
                        
                        cx = rx + halfDimX;
                        cy = ry + halfDimY;
                        cz = rz + halfDimZ;
                    }
                    cx = roundf(cx);
                    cy = roundf(cy);
                    cz = roundf(cz);

                    if(cx>-1 && cx<shadowCaster->maxDimension && cy>-1 && cy<shadowCaster->maxDimension && cz>-1 && cz<shadowCaster->maxDimension){
                        o = (cx + cz * shadowCaster->maxDimension + cy * shadowCaster->maxDimension * shadowCaster->maxDimension);
                        if(shadowCaster->model[o]!=0){
                            shadowVal = 0;
                        }
                    }
                    finalShadow = shadowVal;
                }else{
                    if(z<obj->dimension[2]-1){ //Up
                        dir = (x + (z+1) * obj->maxDimension + y * obj->maxDimension * obj->maxDimension);
                        finalShadow = obj->model[dir]==0? shadowVal:1;
                    }
                }
                //lighting => 8bits  [1-Empty] [3-Occlusion][2-Direct Light(2), Ambient(1) and self shadow(0)] [1-Shadow from caster]
                obj->lighting[index] = (unsigned char)(obj->lighting[index] & (0b11111110  | (finalShadow&1)));
            }
        }
    }
}