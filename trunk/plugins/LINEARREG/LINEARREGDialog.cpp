/*
 *  Qtstalker stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include "LINEARREGDialog.h"
#include "Util.h"

LINEARREGDialog::LINEARREGDialog (QHash<QString, void *> objects, QStringList methods, QString name) : Dialog (0, name)
{
  QStringList tl;
  QDir dir(QDir::homePath());
  tl << dir.absolutePath() << QString("OTA") << QString("LINEARREG") << QString("settings") << QString("dialog");
  _settingsPath = tl.join("/");
  
  createTab(objects, methods);
  loadSettings();
}

LINEARREGDialog::~LINEARREGDialog ()
{
  saveSettings();
}

void
LINEARREGDialog::createTab (QHash<QString, void *> l, QStringList methods)
{
  QHash<QString, void *> ol;
  QHashIterator<QString, void *> it(l);
  while (it.hasNext())
  {
    it.next();
    Object *o = (Object *) it.value();

    if (o->hasOutput())
      ol.insert(it.key(), it.value());
  }

  QWidget *w = new QWidget;
  
  QFormLayout *form = new QFormLayout;
  form->setSpacing(2);
  form->setMargin(10);
  w->setLayout(form);

  _input = new InputObjectWidget;
  _input->setObjects(ol);
  connect(_input, SIGNAL(valueChanged()), this, SLOT(modified()));
  form->addRow(tr("Input"), _input);
  
  // method
  _method = new QComboBox;
  _method->addItems(methods);
  connect(_method, SIGNAL(currentIndexChanged(int)), this, SLOT(modified()));
  form->addRow(tr("Method"), _method);
  
  // period
  _period = new QSpinBox;
  _period->setRange(1, 999999);
  connect(_period, SIGNAL(valueChanged(int)), this, SLOT(modified()));
  form->addRow(tr("Period"), _period);

  _tabs->addTab(w, tr("Settings"));
}

void
LINEARREGDialog::done ()
{
  emit signalDone((void *) this);
  Dialog::done();
}

void
LINEARREGDialog::loadSettings ()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::loadSettings(settings);
}

void
LINEARREGDialog::saveSettings()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::saveSettings(settings);
}

void
LINEARREGDialog::setSettings (QString i, QString ik, int p, QString m)
{
  _input->setInput(i);
  _input->setKey(tr("Input"), ik);
  _period->setValue(p);
  _method->setCurrentIndex(_method->findText(m));
}

void
LINEARREGDialog::settings (QString &i, QString &ik, int &p, QString &m)
{
  i = _input->input();
  ik = _input->key(tr("Input"));
  p = _period->value();
  m = _method->currentText();
}
