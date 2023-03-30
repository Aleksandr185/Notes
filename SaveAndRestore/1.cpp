static const qint32 MagicNumber = 759641;

QByteArray DialogExamComissionEdit::saveState() const
{
  const int version = 1;
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);

  stream << qint32(MagicNumber);
  stream << qint32(version);

  stream << d->view->header()->saveState();
  stream << this->saveGeometry();

  return data;
}

bool DialogExamComissionEdit::restoreState(const QByteArray& state)
{
  const int version = 1;
  QByteArray sd = state;
  QDataStream stream(&sd, QIODevice::ReadOnly);

  qint32 marker = 0;
  qint32 v = 0;

  stream >> marker;
  stream >> v;

  if ((marker != MagicNumber) || (v != version))
      return false;

  QByteArray header_state;
  stream >> header_state;
  const bool r1 = d->view->header()->restoreState(header_state);

  QByteArray geometry_state;
  stream >> geometry_state;
  const bool r2 = this->restoreGeometry(geometry_state);

  return r1 && r2;
}
