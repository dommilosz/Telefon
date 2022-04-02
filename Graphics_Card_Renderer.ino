void i2cGcardRenderLoop(void *arg) {
  while (true) {
    i2cGcardRender();
    coop_idle(500);
  }
}

void i2cGcardRender() {
  gI2C.InitIfNot();
  
  gI2C.Objects_Create(1, 2);
  gI2C.Objects_Move(1, 0, 0, 0);
  gI2C.Objects_TextAsset(1, 11);

  gI2C.Objects_Create(11, 2);
  gI2C.Objects_Move(11, 0, 160, 0);
  gI2C.Objects_TextAsset(11, 12);

  for (int i = 0; i < 10; i++) {
    GC_DrawMenuField(i + 1);
  }
  DrawCurrentMenuGC();
  if (gc_changed) {
    gI2C.Redraw();
    gc_changed = false;
  }
}

void GC_DrawMenuField(int index) {
  gI2C.Objects_Create(index, 2);
  gI2C.Objects_Move(index, 0, 0, index * 20);
  gI2C.Objects_TextAsset(index, index);
}
