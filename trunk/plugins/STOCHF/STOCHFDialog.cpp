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

#include "STOCHFDialog.h"
#include "Util.h"

STOCHFDialog::STOCHFDialog (QHash<QString, void *> objects, QString name) : Dialog (0, name)
{
  QStringList tl;
  QDir dir(QDir::homePath());
  tl << dir.absolutePath() << QString("OTA") << QString("STOCHF") << QString("settings") << QString("dialog");
  _settingsPath = tl.join("/");
  
  createTab(objects);
  loadSettings();
}

STOCHFDialog::~STOCHFDialog ()
{
  saveSettings();
}

void
STOCHFDialog::createTab (QHash<QString, void *> l)
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
  
  // type
  QStringList tl;
  Util util;
  Object *o = util.object(QString("MA"), QString(), QString());
  if (o)
  {
    QString key("types");
    ObjectCommand toc(key);
    if (o->message(&toc))
      tl = toc.getList(key);
    delete o;
  }
  
  _type = new QComboBox;
  _type->addItems(tl);
  connect(_type, SIGNAL(currentIndexChanged(int)), this, SLOT(modified()));
  form->addRow(tr("MA"), _type);

  // k period
  _kperiod = new QSpinBox;
  _kperiod->setRange(1, 999999);
  connect(_kperiod, SIGNAL(valueChanged(int)), this, SLOT(modified()));
  form->addRow(tr("K Period"), _kperiod);

  // d period
  _dperiod = new QSpinBox;
  _dperiod->setRange(1, 999999);
  connect(_dperiod, SIGNAL(valueChanged(int)), this, SLOT(modified()));
  form->addRow(tr("D Period"), _dperiod);

  _tabs->addTab(w, tr("Settings"));
}

void
STOCHFDialog::done ()
{
  emit signalDone((void *) this);
  Dialog::done();
}

void
STOCHFDialog::loadSettings ()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::loadSettings(settings);
}

void
STOCHFDialog::saveSettings()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::saveSettings(settings);
}

void
STOCHFDialog::setSettings (QString i, QString hk, QString lk, QString ck, QString ma, int kp, int dp)
{
  _input->setInput(i);
  _input->setKey(tr("High"), hk);
  _input->setKey(tr("Low"), lk);
  _input->setKey(tr("Close"), ck);
  _type->setCurrentIndex(_type->findText(ma));
  _kperiod->setValue(kp);
  _dperiod->setValue(dp);
}

void
STOCHFDialog::settings (QString &i, QString &hk, QString &lk, QString &ck, QString &ma, int &kp, int &dp)
{
  i = _input->input();
  hk = _input->key(tr("High"));
  lk = _input->key(tr("Low"));
  ck = _input->key(tr("Close"));
  ma = _type->currentText();
  kp = _kperiod->value();
  dp = _dperiod->value();
}
