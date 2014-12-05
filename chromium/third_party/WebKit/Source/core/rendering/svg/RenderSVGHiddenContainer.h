/*
 * Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef RenderSVGHiddenContainer_h
#define RenderSVGHiddenContainer_h

#include "core/rendering/svg/RenderSVGContainer.h"

namespace blink {

class SVGElement;

// This class is for containers which are never drawn, but do need to support style
// <defs>, <linearGradient>, <radialGradient> are all good examples
class RenderSVGHiddenContainer : public RenderSVGContainer {
public:
    explicit RenderSVGHiddenContainer(SVGElement*);

    virtual const char* renderName() const override { return "RenderSVGHiddenContainer"; }

protected:
    virtual void layout() override;

    virtual bool isOfType(RenderObjectType type) const override { return type == RenderObjectSVGHiddenContainer || RenderSVGContainer::isOfType(type); }

private:
    virtual void paint(PaintInfo&, const LayoutPoint&) override final;
    virtual LayoutRect clippedOverflowRectForPaintInvalidation(const RenderLayerModelObject*, const PaintInvalidationState* = 0) const override final { return LayoutRect(); }
    virtual void absoluteQuads(Vector<FloatQuad>&, bool* wasFixed) const override final;

    virtual bool nodeAtFloatPoint(const HitTestRequest&, HitTestResult&, const FloatPoint& pointInParent, HitTestAction) override final;
};
}

#endif // RenderSVGHiddenContainer_h
