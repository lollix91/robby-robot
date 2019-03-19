void orienta_testa() {
  head_horiz.write(Testa.horiz);
  head_vert.write(Testa.vert);
  Testa.changed = false;
}

void pos_testa(int horiz, int vert) {
  Testa.horiz = horiz;
  Testa.vert = vert;
  Testa.changed = true;
}
