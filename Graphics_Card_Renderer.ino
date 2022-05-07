void i2cGcardRenderLoop(void *arg) {
  while (true) {
    i2cGcardRender();
    coop_idle(500);
  }
}

int color_index = 0;
void i2cGcardRender() {
  if (gI2C.IsConnected() == 0)return;
  gI2C.InitIfNot();

  gI2C.Objects_Create(14, 2);
  gI2C.Objects_Move(14, 0, 0, 0);
  gI2C.Objects_TextAsset(14, 11);

  gI2C.Objects_Create(11, 2);
  gI2C.Objects_Move(11, 0, 160, 0);
  gI2C.Objects_TextAsset(11, 12);

  for (int i = 0; i < 10; i++) {
    GC_DrawMenuField(i + 1);
  }
  DrawCurrentMenuGC();
  if (gc_changed) {
    //gI2C.Redraw();
    gc_changed = false;
  }

  gI2C.Objects_Create(15, 1);
  gI2C.Objects_Move(15, 0, 500, 380);
  gI2C.Objects_Resize(15, 0, 5, 5);
  gI2C.Objects_Color(15, (color_index >> 1) % 2 == 0 ? 28 : 0);
  color_index++;
}

void GC_DrawMenuField(int index) {
  gI2C.Objects_Create(index, 2);
  gI2C.Objects_Move(index, 0, 0, index * 20);
  gI2C.Objects_TextAsset(index, index);
}
